// hzxynox
#pragma once

#include <cassert>

namespace cstd
{
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
    
    // This type will be a pseudo-twin to a standard unique_ptr
    template <typename T>
    class unique_ref
    {
    public:
        unique_ref() = delete;
        
        template <typename... Args>
        unique_ref(Args&&... args)
        {
            m_ptr = new T(forward<Args>(args)...);
        }

        ~unique_ref()
        {
            if (m_ptr) delete m_ptr;
        }

        unique_ref(unique_ref&) = delete;
        unique_ref& operator=(const unique_ref&) = delete;
        unique_ref& operator=(unique_ref&) = delete;
        
        unique_ref(unique_ref&& other) noexcept : m_ptr(other.m_ptr)
        {
            other.m_ptr = nullptr;
        }
        
        unique_ref& operator=(unique_ref&& other) noexcept
        {
            if (other.m_ptr)
            {
                if (m_ptr) delete m_ptr;
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }
            return *this;
        }
        
        template <class U>
        constexpr static remove_reference<U>::type&& move(U&& arg) noexcept
        {
            return static_cast<remove_reference<U>::type&&>(arg);
        }
        
        const T& get() const
        {
            assert(m_ptr);
            return *m_ptr;
        }
        T* operator->() const
        {
            assert(m_ptr);
            return m_ptr;
        }
        
    private:
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

        T* m_ptr = nullptr;
    };
    
}
