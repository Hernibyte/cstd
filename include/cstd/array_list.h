// hzxynox
#pragma once

#include <cassert>
#include <cstd/unique_ref.h>
#include <cstd/utility.h>
#include <new>

namespace cstd
{
    
    template<typename Type>
    class array_list
    {
    public:
        array_list() = default;
        
        template<typename... Args>
        explicit array_list(Args&&... args) :
            m_values(new Type[sizeof...(Args)]{ forward<Args>(args)... }),
            m_size(sizeof...(Args)),
            m_capacity(sizeof...(Args))
        {
        }
        
        // Copia
        array_list(const array_list& other) :
            m_values(new Type[other.m_size]),
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
            
            Type* new_values = new Type[new_capacity];
            
            for (size_t i = 0; i < m_size; i++)
            {
                new_values[i] = move(m_values[i]);
            }
            
            delete[] m_values;
            m_values = new_values;
            m_capacity = new_capacity;
        }
        
        void resize(const size_t new_size, const Type& new_value = Type{}) 
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
        
        size_t      size()      const noexcept { return m_size; }
        size_t      capacity()  const noexcept { return m_size; }
        const Type* begin()     const noexcept { return m_values; }
        const Type* end()       const noexcept { return m_values + m_size; }
        Type*       begin()           noexcept { return m_values; }
        Type*       end()             noexcept { return m_values + m_size; }
        
        // copy
        array_list& operator=(const array_list& other) noexcept
        {
            if (this != &other)
            {
                Type* new_values = new Type[other.m_size];
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
        
        Type& operator[](size_t index)
        {
            assert(index <= m_size);
            return m_values[index];
        }
        
        const Type& operator[](size_t index) const
        {
            assert(index <= m_size);
            return m_values[index];
        }
        
    private:
        Type*  m_values   = nullptr;
        size_t m_size     = 0;
        size_t m_capacity = 0;
        
    };
    
}
