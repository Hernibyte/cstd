// hzxynox

/**
 * @headerfile array.h
 */

#pragma once
#include <assert.h>
#include <functional>
#include <cstd/utility.h>

namespace cstd
{
    /**
     * 
     * @tparam T Value type.
     * @tparam Size Size of array.
     */
    template <typename T, size_t Size>
    class array
    {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        
        /** 
         * @brief Default constructor.
         */
        array() = default;

        /**
         * 
         * @tparam Args List of constructor arguments.
         * @param args Pass throw of arguments.
         */
        template<typename... Args>
        explicit constexpr array(Args&&... args) : 
            m_values{forward<Args>(args)...}, 
            m_size(Size)
        {
            static_assert(sizeof...(Args) <= Size, "Argument amount should be less or equal to Size");
        }

        /**
         * @brief Constructor by copy.
         * @param other Array to be copied.
         */
        array(const array& other) noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                m_values[i] = other.m_values[i];
            }
        }

        /**
         * @brief Constructor by movement.
         * @param other Array to be moved.
         */
        array(array&& other) noexcept :
            m_values{}
        {
            for (size_t i = 0; i < Size; ++i)
            {
                m_values[i] = move(other.m_values[i]);
            }
        }

        /**
         * @brief Concat this with another and return a new array.
         * @tparam OtherSize Size of the other array (Normally be deduced).
         * @param other Array to concat.
         * @return New array with the concat result.
         */
        template <size_t OtherSize>
        array<value_type, Size + OtherSize> concat(array<value_type, OtherSize>& other) const noexcept
        {
            array<value_type, Size + OtherSize> result;
            
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

        /**
         * @brief Determine whether all elements meet the condition.
         * @param predicate Condition.
         * @return True if the condition apply for all elements.
         */
        [[nodiscard]]
        bool every(std::function<bool(const_reference element)>& predicate) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (!func(m_values[i]))
                    return false;
            }
            return true;
        }

        /**
         * @brief Determines if at least one element meet the condition.
         * @param predicate Condition.
         * @return True if the condition apply for at least one element.
         */
        [[nodiscard]]
        bool some(std::function<bool(const_reference element)>& predicate) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (func(m_values[i]))
                    return true;
            }
            return false;
        }

        /**
         * @brief Find the first element that match the condition.
         * @param predicate Condition
         * @return Reference of element. Return nullptr if it's not found.
         */
        [[nodiscard]]
        reference find(std::function<bool(const_reference element)>& predicate) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (func(m_values[i])) return m_values[i];
            }
            return nullptr;
        }

        /**
         * @brief Find the index of the first element that match the condition
         * @param predicate Condition
         * @return Index of element. Return -1 if it's not found.
         */
        [[nodiscard]]
        size_t find_index(std::function<bool(const_reference element)>& predicate) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (func(m_values[i])) return i;
            }
            return -1;
        }

        /**
         * @brief Iterate the array in reverse order and find the first element that match the condition.
         * @param predicate Condition.
         * @return Reference of element. Return nullptr if it's not found.
         */
        [[nodiscard]]
        reference find_last(std::function<bool(const_reference element)>& predicate) const noexcept
        {
            for (size_t i = Size - 1; i > -1; --i)
            {
                if (func(m_values[i])) return m_values[i];
            }
            return nullptr;
        }

        /**
         * @brief Iterate the array in reverse order and find the index of first element that match the condition.
         * @param predicate Condition.
         * @return Index of element. Returns -1 if it's not found.
         */
        [[nodiscard]]
        size_t find_last_index(std::function<bool(const_reference element)>& predicate) const noexcept
        {
            for (size_t i = Size - 1; i > -1; --i)
            {
                if (predicate(m_values[i])) return i;
            }
            return -1;
        }

        /**
         * @brief Determines whether an array contains a certain value among its entries.
         * @param element Element to be found.
         * @return True if the element has been found.
         */
        bool contains(const_reference element) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (m_values[i] == element) return true;
            }
            return false;
        }

        /**
         * @brief Returns the first index at which a given element can be found in the array.
         * @param element Element to be found.
         * @return Index of element. Returns -1 if it's not found.
         */
        [[nodiscard]]
        size_t index_of(const_reference element) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                if (m_values[i] == element) return i;
            }
            return -1;
        }

        /**
         * 
         * @return Return a reversed copy of this array. 
         */
        [[nodiscard]]
        array<value_type, Size> to_reversed() const noexcept
        {
            array<value_type, Size> result = *this;
            pointer left  = result.begin();
            pointer right = result.end() - 1;
            
            while (left < right)
            {
                value_type temp = static_cast<value_type&&>(*left);
                *left  = static_cast<value_type&&>(*right);
                *right = static_cast<value_type&&>(temp);
            
                ++left;
                --right;
            }
            
            return result;
        }

        /**
         * @brief Reverse all elements of this array in place.
         * @return Reference of the same array reversed.
         */
        array& reverse() noexcept
        {
            pointer left  = begin();
            pointer right = end() - 1;
            
            while (left < right)
            {
                value_type temp = static_cast<value_type&&>(*left);
                *left  = static_cast<value_type&&>(*right);
                *right = static_cast<value_type&&>(temp);
            
                ++left;
                --right;
            }
            
            return *this;
        }

        /**
         * @brief A foreach! well, almost. 
         * @param consumer Receives each element of the array for manipulation.
         */
        void for_each(const std::function<void(const_reference element)>& consumer) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                consumer(m_values[i]);
            }
        }

        /**
         * @brief A foreach! well, almost. 
         * @param consumer Receives each element and index of the array for manipulation.
         */
        void for_each(const std::function<void(const_reference element, size_t index)>& consumer) const noexcept
        {
            for (size_t i = 0; i < Size; ++i)
            {
                consumer(m_values[i], i);
            }
        }

        /**
         * @brief Find the element of that index and returns a new array without that element.
         * @param index Index of the element we want to remove.
         * @return New array without the element of that index.
         */
        array<value_type, Size - 1> to_removed_at(const size_t index) noexcept
        {
            array<value_type, Size - 1> result;
            
            for (size_t i = 0; i < Size; ++i)
            {
                if (i == index)
                    continue;
                
                result[i] = m_values[i];
            }
            
            return result;
        }

        /**
         * 
         * @return Reference to first element of array
         */
        reference get_first() noexcept
        {
            return m_values[0];
        }

        /**
         * 
         * @return Reference to last element of array
         */
        reference get_last() noexcept
        {
            return m_values[m_size - 1];
        }
        
        /**
         * @brief Get reference of element given by index.
         * [Warning] This method not check the index range unlike: operator[](const size_t index).
         * @see operator[](const size_t index)
         * @param index Index of element.
         * @return Reference of element given by index.
         */
        reference at(const size_t index) noexcept
        {
            return m_values[index];
        }
        
        /**
         * @brief Get const reference of element given by index.
         * [Warning] This method not check the index range unlike: operator[](const size_t index).
         * @see operator[](const size_t index)
         * @param index Index of element.
         * @return Const reference of element given by index.
         */
        const_reference at(const size_t index) const noexcept
        {
            return m_values[index];
        }

        /**
         * 
         * @return Size of array.
         */
        size_t size() const noexcept { return m_size; };

        /**
         * 
         * @return Begin const iterator.
         */
        const_pointer begin() const noexcept { return m_values; };

        /**
         * 
         * @return End const iterator.
         */
        const_pointer end() const noexcept { return m_values + m_size; };

        /**
         * 
         * @return Begin iterator.
         */
        pointer begin() noexcept { return m_values; };

        /**
         * 
         * @return End iterator.
         */
        pointer end() noexcept { return m_values + m_size; };

        /**
         * @brief Set the element given by index. Pass element by copy.
         * @param index Index of element to set.
         * @param new_element New element to set.
         */
        void set(const size_t index, const_reference new_element) noexcept
        {
            m_values[index] = new_element;
        }

        /**
         * @brief Set the element given by index. Pass element by movement.
         * @param index Index of element to set.
         * @param new_element New element to set.
         */
        void set(const size_t index, value_type&& new_element) noexcept
        {
            m_values[index] = move(new_element);
        }

        /** 
         * 
         * @param other Other array to set this by copy.
         * @return Reference of this array.
         */
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

        /**
         * 
         * @param other Other array to set this by movement.
         * @return Reference of this array.
         */
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

        /**
         * @brief Get reference of element given by index.
         * [Warning] This method check the index range unlike: at(const size_t index).
         * @see at(const size_t index)
         * @param index Index of element.
         * @return Reference of element given by index.
         */
        reference operator[](const size_t index)
        {
            assert(index <= Size);
            return m_values[index];
        }

        /**
         * @brief Get const reference of element given by index.
         * [Warning] This method check the index range unlike: at(const size_t index).
         * @see at(const size_t index)
         * @param index Index of element.
         * @return Const reference of element given by index.
         */
        const_reference operator[](const size_t index) const
        {
            assert(index <= Size);
            return m_values[index];
        }
        
    private:
        value_type m_values[Size];
        size_t     m_size = 0;
        
    };
    
}
