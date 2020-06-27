#pragma once
#include <crtdefs.h>

template <class T, size_t BlockSize = 4096>
class MemPool
{
public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template <class U>
    struct rebind
    {
        typedef MemPool<U> other;
    }

    allocator() noexcept;
    MemPool(const MemPool &other) noexcept;
    template <class U>
    MemPool(const MemPool<U> &other) noexcept;

    ~MemPool();

    pointer address(reference x) const;
    const_pointer address(const_reference x) const;
    pointer allocate(size_type n, const void *hint = 0);
    void deallocate(T *p, std::size_t n);
    size_type max_size() const noexcept;
    template <class U, class... Args>
    void construct(U *p, Args &&... args);
    template <class U>
    void destroy(U *p);
};