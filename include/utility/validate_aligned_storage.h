/*!
    \file validate_aligned_storage.h
    \brief Aligned storage validator definition
    \author Tarcisio Genaro Rodrigues
    \date 29.10.2020
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_VALIDATE_ALIGNED_STORAGE_H
#define CPPCOMMON_UTILITY_VALIDATE_ALIGNED_STORAGE_H

namespace CppCommon {

//! Aligned storage validator
template <size_t StorageSize, size_t StorageAlign, size_t ImplSize, size_t ImplAlign, class Enable = void>
class ValidateAlignedStorage;

//! Aligned storage validator (specialization)
template <size_t StorageSize, size_t StorageAlign, size_t ImplSize, const size_t ImplAlign>
class ValidateAlignedStorage<StorageSize, StorageAlign, ImplSize, ImplAlign, typename std::enable_if<(StorageSize >= ImplSize) && ((StorageAlign % ImplAlign) == 0)>::type> {};

} // namespace CppCommon

#endif // CPPCOMMON_UTILITY_VALIDATE_ALIGNED_STORAGE_H
