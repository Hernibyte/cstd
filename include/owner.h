// hzxynox
#pragma once

namespace cstd
{
    
    // This type work as absolute owner of a ptr. Cannot be transfer, copy o dereferenced manually, He creates and delete the ptr automatically.
    template <typename T>
    class owner
    {
    public:
        template <typename... Args>
        explicit owner(Args&&... args)
        {
            m_ptr = new T(forward<Args>(args)...);
        }

        ~owner()
        {
            delete m_ptr;
        }

        owner(const owner&) = delete;
        owner(owner&&) = default;
        owner& operator=(const owner&) = delete;
        owner& operator=(owner&&) = delete;
        
        const T& get() const { return *m_ptr; }
        const T* operator->() const { return m_ptr; }

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