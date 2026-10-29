// hzxynox
#pragma once

namespace cstd
{
    
#ifdef _WIN64
    typedef unsigned __int64 size_t;
#else
    typedef unsigned int     size_t;
#endif
    
    template <class>
        constexpr static bool is_lvalue_reference = false;
    template <class U>
    constexpr static bool is_lvalue_reference<U&> = true;

    template <class U>
    constexpr static U&& forward(U&& arg) noexcept
    {
        return static_cast<U&&>(arg);
    }

    template <class U>
    constexpr static U&& forward(U& arg) noexcept
    {
        static_assert(!is_lvalue_reference<U>, "bad forward call");
        return static_cast<U&&>(arg);
    }
    
    template <class U>
    struct remove_reference
    {
        using type = U;
    };
    
    template <class U>
    struct remove_reference<U&>
    {
        using type = U;
    };
    
    template <class U>
    struct remove_reference<U&&>
    {
        using type = U;
    };
    
    template <class U>
    constexpr remove_reference<U>::type&& move(U&& arg) noexcept
    {
        return static_cast<remove_reference<U>::type&&>(arg);
    }
    
}
