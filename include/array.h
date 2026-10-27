// hzxynox
#pragma once
#include <assert.h>
#include <utility>

namespace cstd
{
    
    template <typename T, std::size_t Size>
    class array
    {
    public:
        using value_type = T;
        using pointer = T*;
        
        array() = delete;
        
        template<typename... Args>
        constexpr array(Args&&... args) : m_values{forward<Args>(args)...}
        {
            static_assert(sizeof...(Args) <= Size, "Argument amount should be less or equal to Size");
        }
        
        [[nodiscard]] T* at(std::size_t index) noexcept
        {
            return m_values[index];
        }
        
        [[nodiscard]] T& operator[] (std::size_t index)
        {
            assert(index <= Size);
            return m_values[index];
        }
        
    private:
        value_type m_values[Size];
        
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
    };
    
}
