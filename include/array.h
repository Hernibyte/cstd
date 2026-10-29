// hzxynox
#pragma once
#include <assert.h>
#include <functional>

namespace cstd
{
    
#ifdef _WIN64
    typedef unsigned __int64 size_t;
#else
    typedef unsigned int     size_t;
#endif
    
    template <typename T, size_t Size>
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
        
        void for_each(const std::function<void(const T& element)> func) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                func(m_values[i]);
            }
        }
        
        void for_each(const std::function<void(const T& element, size_t index)> func) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                func(m_values[i], i);
            }
        }
        
        [[nodiscard]] T* at(size_t index) noexcept
        {
            return m_values[index];
        }
        
        [[nodiscard]] T& operator[] (size_t index)
        {
            assert(index <= Size);
            return m_values[index];
        }
        
        const T* begin() const noexcept { return m_values; };
        const T* end() const noexcept { return m_values + Size; };
        
        size_t size() const noexcept { return Size; };
        
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
