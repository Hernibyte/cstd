// hzxynox
#pragma once

#include <cstd/utility.h>

namespace cstd
{
    
    // This type work as absolute owner of a ptr. Cannot be transfer, copy o dereferenced manually, He creates and delete the ptr automatically.
    template <typename T>
    class owner_ref
    {
    public:
        template <typename... Args>
        explicit owner_ref(Args&&... args)
        {
            m_ptr = new T(forward<Args>(args)...);
        }

        ~owner_ref()
        {
            delete m_ptr;
        }

        owner_ref(const owner_ref&) = delete;
        owner_ref(owner_ref&&) = default;
        owner_ref& operator=(const owner_ref&) = delete;
        owner_ref& operator=(owner_ref&&) = delete;
        
        const T& get() const { return *m_ptr; }
        T* operator->() const { return m_ptr; }

    private:

        T* m_ptr = nullptr;
    };

}
