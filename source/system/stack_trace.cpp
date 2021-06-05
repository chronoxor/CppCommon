/*!
    \file stack_trace.cpp
    \brief Stack trace snapshot provider implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

// Workaround for the "binutils/bfd.h wants config.h now?" issue
// https://stackoverflow.com/questions/11748035/binutils-bfd-h-wants-config-h-now
#define PACKAGE "CppCommon"
#define PACKAGE_VERSION "1.0.0.0"

#include "system/stack_trace.h"

#include "threads/critical_section.h"
#include "utility/countof.h"

#include <cstring>
#include <iomanip>
#include <sstream>

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
#include <execinfo.h>
#if defined(LIBBFD_SUPPORT)
#include <bfd.h>
#endif
#if defined(LIBDL_SUPPORT)
#include <cxxabi.h>
#include <dlfcn.h>
#endif
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#if defined(DBGHELP_SUPPORT)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4091) // C4091: 'keyword' : ignored on left of 'type' when no variable is declared
#endif
#include <dbghelp.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#endif
#endif

namespace CppCommon {

std::ostream& operator<<(std::ostream& os, const StackTrace::Frame& frame)
{
    // Format stack trace frame address
    std::ios_base::fmtflags flags = os.flags();
    os << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2 * sizeof(uintptr_t)) << (uintptr_t)frame.address << ": ";
    os.flags(flags);
    // Format stack trace frame other fields
    os << (frame.module.empty() ? "<unknown>" : frame.module) << '!';
    os << (frame.function.empty() ? "??" : frame.function) << ' ';
    os << frame.filename;
    if (frame.line > 0)
        os << '(' << frame.line << ')';
    return os;
}

StackTrace::StackTrace(int skip)
{
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
    const int capacity = 1024;
    void* frames[capacity];

    // Capture the current stack trace
    int captured = backtrace(frames, capacity);
    int index = skip + 1;
    int size = captured - index;

    // Check the current stack trace size
    if (size <= 0)
        return;

    // Resize stack trace frames vector
    _frames.resize(size);

    // Capture stack trace snapshot under the critical section
    static CriticalSection cs;
    Locker<CriticalSection> locker(cs);

    // Fill all captured frames with symbol information
    for (int i = 0; i < size; ++i)
    {
        auto& frame = _frames[i];

        // Get the frame address
        frame.address = frames[index + i];

#if defined(LIBDL_SUPPORT)
        // Get the frame information
        Dl_info info;
        if (dladdr(frames[index + i], &info) == 0)
            continue;

        // Get the frame module
        if (info.dli_fname != nullptr)
        {
            const char* module = std::strrchr(info.dli_fname, '/');
            if (module != nullptr)
                frame.module = module + 1;
        }

        // Get the frame function
        if (info.dli_sname != nullptr)
        {
            // Demangle symbol name if need
            int status;
            char* demangled = abi::__cxa_demangle(info.dli_sname, nullptr, 0, &status);
            if ((status == 0) && (demangled != nullptr))
            {
                frame.function = demangled;
                free(demangled);
            }
            else
                frame.function = info.dli_sname;
        }
#endif
#if defined(LIBBFD_SUPPORT)
        bfd* abfd = nullptr;
        char** matching = nullptr;

        void* symsptr = nullptr;
        asymbol** syms = nullptr;
        unsigned int symsize;
        long symcount;

        const char* filename = nullptr;
        const char* functionname = nullptr;
        unsigned int line;

        bfd_boolean found = false;
        bfd_vma pc;

        if ((frame.address == nullptr) || (info.dli_fname == nullptr))
            continue;

        abfd = bfd_openr(info.dli_fname, nullptr);
        if (abfd == nullptr)
            continue;

        if (bfd_check_format(abfd, bfd_archive))
            goto cleanup;

        if (!bfd_check_format_matches(abfd, bfd_object, &matching))
            goto cleanup;

        if ((bfd_get_file_flags(abfd) & HAS_SYMS) == 0)
            goto cleanup;

        symcount = bfd_read_minisymbols(abfd, FALSE, &symsptr, &symsize);
        if (symcount == 0)
            symcount = bfd_read_minisymbols(abfd, TRUE, &symsptr, &symsize);
        if (symcount < 0)
            goto cleanup;
        syms = (asymbol**)symsptr;

        pc = (bfd_vma)frame.address;
        for (asection* section = abfd->sections; section != nullptr; section = section->next)
        {
            if (found)
                break;

            if ((bfd_section_flags(section) & SEC_ALLOC) == 0)
                continue;

            bfd_vma vma = bfd_section_vma(section);
            if (pc < vma)
                continue;

            bfd_size_type secsize = bfd_section_size(section);
            if (pc >= vma + secsize)
                continue;

            found = bfd_find_nearest_line(abfd, section, syms, pc - vma, &filename, &functionname, &line);
        }

        if (!found)
            goto cleanup;

        if (filename != nullptr)
            frame.filename = filename;
        frame.line = line;

cleanup:
        if (symsptr != nullptr)
            free(symsptr);

        if (abfd != nullptr)
            bfd_close(abfd);
#endif
    }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    const int capacity = 1024;
    void* frames[capacity];

    // Capture the current stack trace
    USHORT captured = CaptureStackBackTrace(skip + 1, capacity, frames, nullptr);

    // Resize stack trace frames vector
    _frames.resize(captured);

    // Capture stack trace snapshot under the critical section
    static CriticalSection cs;
    Locker<CriticalSection> locker(cs);

    // Fill all captured frames with symbol information
    for (int i = 0; i < captured; ++i)
    {
        auto& frame = _frames[i];

        // Get the frame address
        frame.address = frames[i];

#if defined(DBGHELP_SUPPORT)
        // Get the current process handle
        HANDLE hProcess = GetCurrentProcess();

        // Get the frame module
        IMAGEHLP_MODULE64 module;
        ZeroMemory(&module, sizeof(module));
        module.SizeOfStruct = sizeof(module);
        if (SymGetModuleInfo64(hProcess, (DWORD64)frame.address, &module))
        {
            const char* image = std::strrchr(module.ImageName, '\\');
            if (image != nullptr)
                frame.module = image + 1;
        }

        // Get the frame function
        char symbol[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
        ZeroMemory(&symbol, countof(symbol));
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbol;
        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;
        if (SymFromAddr(hProcess, (DWORD64)frame.address, nullptr, pSymbol))
        {
            char buffer[4096];
            if (UnDecorateSymbolName(pSymbol->Name, buffer, (DWORD)countof(buffer), UNDNAME_NAME_ONLY) > 0)
                frame.function = buffer;
        }

        // Get the frame file name and line number
        DWORD offset = 0;
        IMAGEHLP_LINE64 line;
        ZeroMemory(&line, sizeof(line));
        line.SizeOfStruct = sizeof(line);
        if (SymGetLineFromAddr64(hProcess, (DWORD64)frame.address, &offset, &line))
        {
            if (line.FileName != nullptr)
                frame.filename = line.FileName;
            frame.line = line.LineNumber;
        }
#endif
    }
#endif
}

std::ostream& operator<<(std::ostream& os, const StackTrace& stack_trace)
{
    for (const auto& frame : stack_trace.frames())
        os << frame << std::endl;
    return os;
}

} // namespace CppCommon
