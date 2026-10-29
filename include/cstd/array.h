// hzxynox
#pragma once
#include <assert.h>
#include <functional>
#include <cstd/utility.h>

namespace cstd
{
    
    template <typename T, size_t Size>
    class array
    {
    public:
        using value_type = T;
        using pointer = T*;
        
        array() = default;
        
        template<typename... Args>
        explicit constexpr array(Args&&... args) : 
            m_values{forward<Args>(args)...}, 
            m_size(Size)
        {
            static_assert(sizeof...(Args) <= Size, "Argument amount should be less or equal to Size");
        }
        
        // Copia
        array(const array& other) noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                m_values[i] = other.m_values[i];
            }
        }
        
        // Movimiento
        array(array&& other) noexcept :
            m_values{}
        {
            for (size_t i = 0; i < Size; ++i)
            {
                m_values[i] = move(other.m_values[i]);
            }
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
        
        size_t   size()  const noexcept { return m_size; };
        const T* begin() const noexcept { return m_values; };
        const T* end()   const noexcept { return m_values + Size; };
        T*       begin()      noexcept { return m_values; };
        T*       end()        noexcept { return m_values + Size; };
        
        // copy
        array& operator=(const array& other) noexcept
        {
            if (this != &other)
            {
                for (size_t i = 0; i < Size; ++i)
                {
                    m_values[i] = other.m_values[i];
                }
            }
            return *this;
        }
        
        // move
        array& operator=(array&& other) noexcept
        {
            if (this != &other)
            {
                for (size_t i = 0; i < Size; ++i)
                {
                    m_values[i] = move(other.m_values[i]);
                }
            }
            return *this;
        }
        
        T& operator[] (size_t index)
        {
            assert(index <= Size);
            return m_values[index];
        }
        
        const T& operator[] (size_t index) const
        {
            assert(index <= Size);
            return m_values[index];
        }
        
    private:
        value_type m_values[Size];
        size_t m_size = 0;
        
    };
    
}
