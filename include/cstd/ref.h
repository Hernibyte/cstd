// hzxynox
#pragma once

#include <cstd/utility.h>

namespace cstd
{
    
    // This type will be a pseudo-twin to a standard shared_ptr
    template<typename T>
    class ref
    {
    private:
        template <class U>
        struct value
        {
            template <typename... Args>
            explicit value(Args&&... args) noexcept
            {
                m_ptr = new U(forward<Args>(args)...);
                m_count++;
            }
            
            ~value()
            {
                delete m_ptr;
            }
            
            unsigned int m_count = 0;
            U* m_ptr = nullptr;
        };
        
    public:
        explicit ref()
        {
            m_value_ptr = new value<T>();
        }
        
        template <typename... Args>
        explicit ref(Args&&... args)
        {
            m_value_ptr = new value<T>(forward<Args>(args)...);
        }
        
        ref(ref& other)
        {
            m_value_ptr = other.m_value_ptr;
            ++m_value_ptr->m_count;
        }
        
        ~ref()
        {
            --m_value_ptr->m_count;
            if (m_value_ptr->m_count == 0)
            {
                delete m_value_ptr;
            }
        }
        
        unsigned int count() const { return m_value_ptr->m_count; }
        
        const T& get() const
        {
            return *m_value_ptr->m_ptr;
        }
        T* operator->() const
        {
            return m_value_ptr->m_ptr;
        }
        
    private:
        value<T>* m_value_ptr = nullptr;
        
    };
    
}
