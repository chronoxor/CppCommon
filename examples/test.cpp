#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>
#include <string.h>
#endif

#if defined(__APPLE__)
#include <mach/mach.h>
#include <sys/sysctl.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <iostream>
#include <vector>

void RandomFill(void* buffer, size_t size)
{
#if defined(unix) || defined(__unix) || defined(__unix__)
    int fd = open("/dev/urandom", O_RDONLY);
    ssize_t count = read(fd, buffer, size);
    int result = close(fd);
#elif defined(_WIN32) || defined(_WIN64)
    char* ptr = (char*)buffer;
    for(size_t i = 0; i < size; ++i)
        ptr[i] = rand() % 256;
#endif
}

int main(int argc, char** argv)
{
    std::cout << "Press Enter to stop..." << std::endl;

    std::string value(16, 0);
    RandomFill(value.data(), value.size());

    std::cout << "Random:" << value << std::endl;

    return 0;
}
