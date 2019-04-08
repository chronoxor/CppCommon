/*!
    \file locker.h
    \brief Locker synchronization primitive definition
    \author Ivan Shynkarenka
    \date 08.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_LOCKER_H
#define CPPCOMMON_THREADS_LOCKER_H

namespace CppCommon {

//! Locker synchronization primitive
/*!
    A locker is an object that manages a given synchronization primitive by keeping it
    always locked (lock in constructor and unlock in destructor).

    Thread-safe.
*/
template <class T>
class Locker
{
public:
    //! Default class constructor
    /*!
        \param primitive - Synchronization primitive to manage
    */
    explicit Locker(T& primitive) : _primitive(primitive) { _primitive.Lock(); }
    Locker(const Locker&) = delete;
    Locker(Locker&&) = delete;
    ~Locker() { _primitive.Unlock(); }

    Locker& operator=(const Locker&) = delete;
    Locker& operator=(Locker&&) = delete;

private:
    T& _primitive;
};

//! Read locker synchronization primitive
/*!
    A read locker is an object that manages a given read/write synchronization primitive by keeping it
    always locked for read (read-lock in constructor and read-unlock in destructor).

    Thread-safe.
*/
template <class T>
class ReadLocker
{
public:
    //! Default class constructor
    /*!
        \param primitive - Synchronization primitive to manage
    */
    explicit ReadLocker(T& primitive) : _primitive(primitive) { _primitive.LockRead(); }
    ReadLocker(const ReadLocker&) = delete;
    ReadLocker(ReadLocker&&) = delete;
    ~ReadLocker() { _primitive.UnlockRead(); }

    ReadLocker& operator=(const ReadLocker&) = delete;
    ReadLocker& operator=(ReadLocker&&) = delete;

private:
    T& _primitive;
};

//! Write locker synchronization primitive
/*!
    A write locker is an object that manages a given read/write synchronization primitive by keeping it
    always locked for write (write-lock in constructor and write-unlock in destructor).

    Thread-safe.
*/
template <class T>
class WriteLocker
{
public:
    //! Default class constructor
    /*!
        \param primitive - Synchronization primitive to manage
    */
    explicit WriteLocker(T& primitive) : _primitive(primitive) { _primitive.LockWrite(); }
    WriteLocker(const WriteLocker&) = delete;
    WriteLocker(WriteLocker&&) = delete;
    ~WriteLocker() { _primitive.UnlockWrite(); }

    WriteLocker& operator=(const WriteLocker&) = delete;
    WriteLocker& operator=(WriteLocker&&) = delete;

private:
    T& _primitive;
};

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_LOCKER_H
