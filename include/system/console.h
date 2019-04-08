/*!
    \file console.h
    \brief Console management definition
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_CONSOLE_H
#define CPPCOMMON_SYSTEM_CONSOLE_H

#include <ostream>

namespace CppCommon {

//! Supported console colors
enum class Color
{
    BLACK,          //!< Black color
    BLUE,           //!< Blue color
    GREEN,          //!< Green color
    CYAN,           //!< Cyan color
    RED,            //!< Red color
    MAGENTA,        //!< Magenta color
    BROWN,          //!< Brown color
    GREY,           //!< Grey color
    DARKGREY,       //!< Dark grey color
    LIGHTBLUE,      //!< Light blue color
    LIGHTGREEN,     //!< Light green color
    LIGHTCYAN,      //!< Light cyan color
    LIGHTRED,       //!< Light red color
    LIGHTMAGENTA,   //!< Light magenta color
    YELLOW,         //!< Yellow color
    WHITE           //!< White color
};

//! Stream manipulator: change console text color
/*!
    \param stream - Output stream
    \param color - Console text color
    \return Output stream
*/
template <class TOutputStream>
TOutputStream& operator<<(TOutputStream& stream, Color color);

//! Stream manipulator: change console text and background colors
/*!
    \param stream - Output stream
    \param colors - Console text and background colors
    \return Output stream
*/
template <class TOutputStream>
TOutputStream& operator<<(TOutputStream& stream, std::pair<Color, Color> colors);

//! Console management static class
/*!
    Provides console management functionality such as setting
    text and background colors.

    Thread-safe.
*/
class Console
{
public:
    Console() = delete;
    Console(const Console&) = delete;
    Console(Console&&) = delete;
    ~Console() = delete;

    Console& operator=(const Console&) = delete;
    Console& operator=(Console&&) = delete;

    //! Set console text color
    /*!
        \param color - Console text color
        \param background - Console background color (default is Color::BLACK)
    */
    static void SetColor(Color color, Color background = Color::BLACK);
};

/*! \example system_console.cpp Console management example */

} // namespace CppCommon

#include "console.inl"

#endif // CPPCOMMON_SYSTEM_CONSOLE_H
