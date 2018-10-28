#ifndef PANNOIREENGINE_MEMORYPOOL_H
#define PANNOIREENGINE_MEMORYPOOL_H

#include <cstddef>
#include <cassert>
#include <vector>
#include <gsl/gsl>

namespace PE::ECS {

    class MemoryPool {
    public:
        explicit MemoryPool(std::size_t t_element_size, std::size_t t_chunk_size = 8192)
                : m_size(0), m_capacity(0), m_element_size(t_element_size), m_chunk_size(t_chunk_size)
        {
            assert(t_chunk_size > 0);
        };

        virtual ~MemoryPool();

        inline void reserve(std::size_t n)
        {
            while(m_capacity < n)
            {
                auto chunk = new std::byte[m_element_size * m_chunk_size];
                m_memory.push_back(chunk);
                m_capacity += m_chunk_size;
            }
        }

        inline void expand(std::size_t n)
        {
            if(n >= m_size)
            {
                if(n >= m_capacity) reserve(n);
                m_size = n;
            }
        }

        inline std::byte *get(std::size_t n) {
            assert(n < m_size);
            return m_memory[n / m_chunk_size] + (n % m_chunk_size) * m_element_size;
        }

        inline const std::byte *get(std::size_t n) const {
            assert(n < m_size);
            return m_memory[n / m_chunk_size] + (n % m_chunk_size) * m_element_size;
        }

        virtual void destroy(std::size_t n) = 0;

    protected:
        std::vector<std::byte *> m_memory;
        std::size_t m_size;
        std::size_t m_capacity; // how much can I contain here
        const std::size_t m_element_size;
        const std::size_t m_chunk_size;
    };

    template <typename T>
    class ObjectPool : public MemoryPool
    {
    public:
        ObjectPool(std::size_t t_chunk_size = 8192)
                : MemoryPool(sizeof(T), t_chunk_size) {};

        virtual ~ObjectPool();

        inline T *get_object(std::size_t n) {
            return reinterpret_cast<T *>(get(n));
        }

        inline const T *get_object(std::size_t n) const {
            return reinterpret_cast<const T *>(get(n));
        }

        // WARNING
        inline T* get_objects() {
            return *(reinterpret_cast<T**>(m_memory.data()));
        }


        virtual void destroy(std::size_t n) override {
            T *object_ptr = reinterpret_cast<T *>(get(n));
            object_ptr->~T();
        }

    };

    template <typename T>
    ObjectPool<T>::~ObjectPool() {
        for(std::size_t i = 0; i < m_size; ++i) {
            destroy(i);
        }
    }
}

#endif //PANNOIREENGINE_MEMORYPOOL_H
