#include <iostream>
#include <limits>
#include <vector>
#include <cassert>

using namespace std;

// ---------- ByteCounter ----------
struct ByteCounterImplement {
    size_t allocated = 0;
    size_t deallocated = 0;
};

class ByteCounter {
private:
    ByteCounterImplement* m_implement;

public:
    ByteCounter(): m_implement(new ByteCounterImplement) {}

    ByteCounter(const ByteCounter& counter): m_implement(counter.m_implement) {}

    ByteCounter(ByteCounter&& counter): m_implement(counter.m_implement) {}

    ~ByteCounter() = default;

    ByteCounter& operator=(const ByteCounter& counter) = delete;
    ByteCounter& operator=(ByteCounter&& counter) = delete;

    void increase(size_t n_size){
        this->m_implement->allocated += n_size;
    }

    void decrease(size_t n_size){
        this->m_implement->deallocated += n_size;
    }

    size_t bytes() const{
        return (this->m_implement->allocated - this->m_implement->deallocated);
    }

    size_t allocated() const{
        return this->m_implement->allocated;
    }

    size_t deallocated() const{
        return this->m_implement->deallocated;
    }

    constexpr bool operator==(const ByteCounter& counter) const{
        return (this->m_implement == counter.m_implement);
    }
};

// ---------- Allocator ----------
template<class T>
class Allocator {
private:
    ByteCounter my_counter;

public:
    using value_type = T;

    Allocator() = default;

    template<class U>
    Allocator(const Allocator<U>& rhs) noexcept : my_counter(rhs.my_counter) {}

    ~Allocator() = default;

    T* allocate(size_t n){
        // check allocate size
        assert(n <= numeric_limits<size_t>::max() / sizeof(T));

        const size_t bytes = n * sizeof(T);
        T* ptr = static_cast<T*>(malloc(bytes));

        // check malloc status
        assert(ptr);

        this->my_counter.increase(bytes);
        cout << "Allocate increase: " << this->my_counter.bytes() << endl;

        return ptr;
    }

    void deallocate(T* ptr, size_t n){
        free(ptr);
        this->my_counter.decrease(n * sizeof(T));
        cout << "Deallocate decrease: " << this->my_counter.bytes() << endl;
    }

    constexpr size_t bytes() const{
        return this->my_counter.bytes();
    }

    constexpr size_t allocated() const{
        return this->my_counter.allocated();
    }

    constexpr size_t deallocated() const{
        return this->my_counter.deallocated();
    }

    constexpr bool operator==(const Allocator<T>& rhs) const{
        return (this->my_counter == rhs.my_counter);
    }

    friend ostream& operator<<(ostream& out, const Allocator<T>& rhs){
        out << "Allocator: bytes = " << rhs.bytes() << '\n'\
            << "               allocated = " << rhs.allocated() << '\n'\
            << "               deallocated = " << rhs.deallocated() << endl;

        return out;
    }
};

// Allocator init in global
Allocator<double> my_allocator;

size_t bytes(){
    return my_allocator.bytes();
}

size_t allocated(){
    return my_allocator.allocated();
}

size_t deallocated(){
    return my_allocator.deallocated();
}
