/*!
    \file singleton.h
    \brief Singleton definition
    \author Ivan Shynkarenka
    \date 31.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_SINGLETON_H
#define CPPCOMMON_UTILITY_SINGLETON_H

namespace CppCommon {

//! Singleton template base class
/*!
    Singleton base class is used to protect child class from being created
    multiple times and provide a static GetInstance() method to access the
    single instance.

    Thread-safe.

    Example:
    \code{.cpp}
    class MySingleton : public CppCommon::Singleton<MySingleton>
    {
       friend CppCommon::Singleton<MySingleton>;

    public:
        void Test() { ... }

    private:
        MySingleton() = default;
        ~MySingleton() = default;
    };

    int main(int argc, char** argv)
    {
        MySingleton::GetInstance().Test();
        return 0;
    }
    \endcode

    https://en.wikipedia.org/wiki/Singleton_pattern
*/
template <typename T>
class Singleton
{
    friend T;

public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;

    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton &&) = delete;

    //! Get singleton instance
    /*!
        \return Singleton instance
    */
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

private:
    Singleton() noexcept = default;
    ~Singleton() noexcept = default;
};

/*! \example utility_singleton.cpp Singleton example */

} // namespace CppCommon

#endif // CPPCOMMON_UTILITY_SINGLETON_H
