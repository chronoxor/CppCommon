/*!
    \file endian.inl
    \brief Big/Little-endian utilities inline implementation
    \author Ivan Shynkarenka
    \date 21.07.2017
    \copyright MIT License
*/

namespace CppCommon {

bool Endian::IsBigEndian()
{
    return false;
}

bool Endian::IsLittleEndian()
{
    return true;
}

} // namespace CppCommon
