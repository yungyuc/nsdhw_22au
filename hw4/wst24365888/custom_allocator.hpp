#ifndef CUSTOM_ALLOCATOR
#define CUSTOM_ALLOCATOR

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <new>
#include <vector>

struct ByteCounterImpl {
    std::size_t allocated = 0;
    std::size_t deallocated = 0;
    std::size_t refcount = 0;
};

class ByteCounter {
public:
    ByteCounter()
        : m_impl(new ByteCounterImpl) {
        incref();
    }

    ByteCounter(ByteCounter const& other)
        : m_impl(other.m_impl) {
        incref();
    }

    ByteCounter& operator=(ByteCounter const& other) {
        if (&other != this) {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter&& other)
        : m_impl(other.m_impl) {
        incref();
    }

    ByteCounter& operator=(ByteCounter&& other) {
        if (&other != this) {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() {
        decref();
    }

    void swap(ByteCounter& other) {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount) {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount) {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const {
        return m_impl->allocated - m_impl->deallocated;
    }
    std::size_t allocated() const {
        return m_impl->allocated;
    }
    std::size_t deallocated() const {
        return m_impl->deallocated;
    }
    std::size_t refcount() const {
        return m_impl->refcount;
    }

private:
    void incref() {
        ++m_impl->refcount;
    }

    void decref() {
        if (m_impl == nullptr) {
            // Do nothing.
        } else if (m_impl->refcount == 1) {
            delete m_impl;
            m_impl = nullptr;
        } else {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl* m_impl;
};

template <class T>
struct CustomAllocator {
    using value_type = T;

    CustomAllocator() = default;

    template <class U>
    constexpr CustomAllocator(const CustomAllocator<U>& other) noexcept {
        counter = other.counter;
    }

    T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n * sizeof(T);
        T* p = static_cast<T*>(std::malloc(bytes));
        if (p) {
            counter.increase(bytes);
            return p;
        } else {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::free(p);

        const std::size_t bytes = n * sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;
};

#endif
