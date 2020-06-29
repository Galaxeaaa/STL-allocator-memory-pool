#pragma once
#include <cstddef>
#include "mempool.h"

template <class T, size_t BlockSize = 4096>
class allocator
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
        typedef allocator<U> other;
    };

    allocator() noexcept {}
    allocator(const allocator &other) noexcept {}
    template <class U>
    allocator(const allocator<U> &other) noexcept {}

    ~allocator() {}

    pointer address(reference x) const {
        return *x;
    }
    
    const_pointer address(const_reference x) const {
        return *x;
    }
    
    pointer allocate(size_type n, const void *hint = 0){
        return (pointer)MemPool::mpAlloc(sizeof(value_type)*n);
    }

    void deallocate(T *p, std::size_t n){
        MemPool::mpFree((void*)p, n);
    }

    size_type max_size() const noexcept{
        // return std::numeric_limits<size_type>::max() / sizeof(value_type);
        size_type sz=2ULL*1024ULL*1024ULL*1024ULL;
        return sz / sizeof(value_type);
    }

    template <class _Up, class... _Args>
    void construct(_Up *_p, _Args &&... _args){
        ::new ((void*)_p) _Up(std::forward<_Args>(_args)...);
    }
    
    template <class _Up>
    void destroy(_Up *_p){
        _p->~_Up();
    }
};