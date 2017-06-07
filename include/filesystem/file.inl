/*!
    \file file.inl
    \brief Filesystem file inline implementation
    \author Ivan Shynkarenka
    \date 29.08.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void File::swap(File& file) noexcept
{
    using std::swap;
    Path::swap(file);
    swap(_pimpl, file._pimpl);
}

inline void swap(File& file1, File& file2) noexcept
{
    file1.swap(file2);
}

} // namespace CppCommon
