/*!
    \file validate_aligned_storage.h
    \brief 
    \author Tarcisio Rodrigues
    \date 18.10.2020
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_VALIDATE_ALIGNED_STORAGE_H
#define CPPCOMMON_UTILITY_VALIDATE_ALIGNED_STORAGE_H

#include <cstddef>
#include <type_traits>

namespace CppCommon {

    template<const size_t ImplSize, const size_t ImplAlign, const size_t StorageSize, const size_t StorageAlign, class Enable = void>
    class ValidateAlignedStorage;

    template<const size_t ImplSize, const size_t ImplAlign, const size_t StorageSize, const size_t StorageAlign>
    class ValidateAlignedStorage
    <
        ImplSize,
        ImplAlign,
        StorageSize,
        StorageAlign,
        typename std::enable_if
        <
            StorageAlign % ImplAlign == 0 && ImplSize <= StorageSize
        >::type
    >
    {
    };


} // namespace CppCommon

#endif // CPPCOMMON_UTILITY_VALIDATE_ALIGNED_STORAGE_H
