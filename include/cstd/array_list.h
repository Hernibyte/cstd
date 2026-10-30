// hzxynox

/**
 * @headerfile array_list.h
 */

#pragma once

#include <cassert>
#include <cstd/unique_ref.h>
#include <cstd/utility.h>

namespace cstd
{
    /**
     * 
     * @tparam T 
     */
    template<typename T>
    class array_list
    {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        
        array_list() = default;
        
        template<typename... Args>
        explicit array_list(Args&&... args) :
            m_values(new value_type[sizeof...(Args)]{ forward<Args>(args)... }),
            m_size(sizeof...(Args)),
            m_capacity(sizeof...(Args))
        {
        }
        
        // Copia
        array_list(const array_list& other) :
            m_values(new value_type[other.m_size]),
            m_size(other.m_size),
            m_capacity(other.m_capacity)
        {
            for (size_t i = 0; i < m_size; i++)
            {
                m_values[i] = other.m_values[i];
            }
        }
        
        // Movimiento
        array_list(array_list&& other) noexcept :
            m_values(exchange(other.m_values, nullptr)),
            m_size(exchange(other.m_size, 0)),
            m_capacity(exchange(other.m_capacity, 0))
        {
        }
        
        ~array_list()
        {
            delete[] m_values;
        }
        
        void reserve(const size_t new_capacity)
        {
            if (new_capacity <= m_capacity) return;
            
            pointer new_values = new value_type[new_capacity];
            
            for (size_t i = 0; i < m_size; i++)
            {
                new_values[i] = move(m_values[i]);
            }
            
            delete[] m_values;
            m_values = new_values;
            m_capacity = new_capacity;
        }
        
        void resize(const size_t new_size, const_reference new_value = value_type{}) 
        {
            if (new_size > m_capacity)
            {
                size_t new_capacity = (m_capacity == 0) ? new_size : m_capacity * 2;
                if (new_capacity < new_size) new_capacity = new_size;
                reserve(new_capacity);
            }
            
            if (new_size > m_size)
            {
                for (size_t i = m_size; i < new_size; i++)
                {
                    m_values[i] = new_value;
                }
            }
            
            m_size = new_size;
        }
        
        reference get_first()
        {
            return m_values[0];
        }
        
        reference get_last()
        {
            return m_values[m_size - 1];
        }
        
        [[nodiscard]] 
        reference at(size_t index) noexcept
        {
            return m_values[index];
        }
        
        size_t        size()      const noexcept { return m_size; }
        size_t        capacity()  const noexcept { return m_size; }
        const_pointer begin()     const noexcept { return m_values; }
        const_pointer end()       const noexcept { return m_values + m_size; }
        pointer       begin()           noexcept { return m_values; }
        pointer       end()             noexcept { return m_values + m_size; }
        
        // copy
        void set(size_t index, const_reference element) noexcept
        {
            m_values[index] = element;
        }
        
        // move
        void set(size_t index, value_type&& element) noexcept
        {
            m_values[index] = move(element);
        }
        
        // copy
        array_list& operator=(const array_list& other) noexcept
        {
            if (this != &other)
            {
                pointer new_values = new value_type[other.m_size];
                for (size_t i = 0; i < other.m_size; i++)
                {
                    new_values[i] = other.m_values[i];
                }
                
                delete[] m_values;
                m_values = new_values;
                m_size = other.m_size;
            }
            return *this;
        }
        
        // move
        array_list& operator=(array_list&& other) noexcept
        {
            if (this != &other)
            {
                delete[] m_values;
                m_values   = exchange(other.m_values, nullptr);
                m_size     = exchange(other.m_size, 0);
                m_capacity = exchange(other.m_capacity, 0);
            }
            return *this;
        }
        
        reference operator[](size_t index)
        {
            assert(index <= m_size);
            return m_values[index];
        }
        
        const_reference operator[](size_t index) const
        {
            assert(index <= m_size);
            return m_values[index];
        }
        
    private:
        pointer  m_values   = nullptr;
        size_t   m_size     = 0;
        size_t   m_capacity = 0;
        
    };
    
}
