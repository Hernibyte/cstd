// hzxynox
#pragma once
#include <algorithm>
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
        
        array() = default;
        
        template<typename... Args>
        explicit constexpr array(Args&&... args) : m_values{forward<Args>(args)...}
        {
            static_assert(sizeof...(Args) <= Size, "Argument amount should be less or equal to Size");
        }
        
        template <size_t OtherSize>
        array<T, Size + OtherSize> concat(array<T, OtherSize>& other) const noexcept
        {
            array<T, Size + OtherSize> result;
            
            for (size_t i = 0; i < Size; ++i)
            {
                result[i] = m_values[i];
            }
            
            for (size_t i = 0; i < OtherSize; ++i)
            {
                result[i + Size] = other[i];
            }
            
            return result;
        }
        
        [[nodiscard]]
        bool every(std::function<bool(const T& element)>& func) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (!func(m_values[i]))
                    return false;
            }
            return true;
        }
        
        [[nodiscard]]
        bool some(std::function<bool(const T& element)>& func) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (func(m_values[i]))
                    return true;
            }
            return false;
        }
        
        [[nodiscard]]
        T& find(std::function<bool(const T& element)>& func) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (func(m_values[i])) return m_values[i];
            }
            return nullptr;
        }
        
        [[nodiscard]]
        size_t find_index(std::function<bool(const T& element)>& func) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (func(m_values[i])) return i;
            }
            return -1;
        }
        
        [[nodiscard]]
        T& find_last(std::function<bool(const T& element)>& func) const noexcept
        {
            for (size_t i = Size - 1; i > -1; --i)
            {
                if (func(m_values[i])) return m_values[i];
            }
            return nullptr;
        }
        
        [[nodiscard]]
        size_t find_last_index(std::function<bool(const T& element)>& func) const noexcept
        {
            for (size_t i = Size - 1; i > -1; --i)
            {
                if (func(m_values[i])) return i;
            }
            return -1;
        }
        
        [[nodiscard]]
        bool includes(const T& element) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (m_values[i] == element) return true;
            }
            return false;
        }
        
        [[nodiscard]]
        size_t index_of(const T& element) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (m_values[i] == element) return i;
            }
            return -1;
        }
        
        [[nodiscard]]
        array<T, Size> to_reversed() const noexcept
        {
            array<T, Size> result = *this;
            T* left  = result.begin();
            T* right = result.end() - 1;
            
            while (left < right)
            {
                T temp = static_cast<T&&>(*left);
                *left  = static_cast<T&&>(*right);
                *right = static_cast<T&&>(temp);
            
                ++left;
                --right;
            }
            
            return result;
        }
        
        void reverse() noexcept
        {
            T* left  = begin();
            T* right = end() - 1;
            
            while (left < right)
            {
                T temp = static_cast<T&&>(*left);
                *left  = static_cast<T&&>(*right);
                *right = static_cast<T&&>(temp);
            
                ++left;
                --right;
            }
        }
        
        
        
        void for_each(const std::function<void(const T& element)>& func) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                func(m_values[i]);
            }
        }
        
        void for_each(const std::function<void(const T& element, size_t index)>& func) const noexcept
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
        T* begin() noexcept { return m_values; };
        T* end() noexcept { return m_values + Size; };
        
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
