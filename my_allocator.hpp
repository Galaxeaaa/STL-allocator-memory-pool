#pragma once
// #include <crtdefs.h>
#include<cstddef>
#include<type_traits>

template <typename _Tp, typename std::size_t BlockSize = 4096>
class MemPool
{
private:
    union _Slot{
      _Tp ele;
      _Slot* next;
    };
    typedef unsigned char* tmp_ptr;
    typedef _Slot  slot_type;
    typedef _Slot* slot_ptr;

    slot_ptr curBlock;
    slot_ptr curSlot;
    slot_ptr lastSlot;
    slot_ptr freeList;
    static_assert(BlockSize >= /* 2 *  */sizeof(slot_ptr)+sizeof(slot_type), "BlockSize too small.");

public:
    typedef _Tp value_type;
    typedef _Tp *pointer;
    typedef const _Tp *const_pointer;
    typedef _Tp &reference;
    typedef const _Tp &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type is_always_equal;

    template <class U>
    struct rebind
    {
        typedef MemPool<U> other;
    };

    MemPool() noexcept{
        curBlock=nullptr;
        curSlot=nullptr;
        lastSlot=nullptr;
        freeList=nullptr;
    }

    MemPool(const MemPool &other) noexcept{
        
    }

    template <class U>
    MemPool(const MemPool<U> &other) noexcept{

    }

    ~MemPool(){
        slot_ptr p=curBlock;
        while (p != nullptr) {
            slot_ptr tmp=p->next;
            ::operator delete(reinterpret_cast<void*>(p));
            p=tmp;
        }
    }

    pointer address(reference x) const noexcept{

    }

    const_pointer address(const_reference x) const noexcept{

    }

    pointer allocate(size_type n=1, const void *hint = 0){
        if(freeList != nullptr){
            pointer ret = reinterpret_cast<pointer>(freeList);
            freeList = freeList->next;
            return ret;
        }else{
            if(curSlot >= lastSlot){
                tmp_ptr newBlock = reinterpret_cast<tmp_ptr>(::operator new(BlockSize));
                reinterpret_cast<slot_ptr>(newBlock)->next = curBlock;
                curBlock = reinterpret_cast<slot_ptr>(newBlock);
                tmp_ptr block_body= newBlock + sizeof(slot_ptr);
                size_t align_gap = (alignof(slot_type) - reinterpret_cast<size_t>(block_body)) % alignof(slot_type);
                curSlot = reinterpret_cast<slot_ptr>(block_body + align_gap);
                lastSlot = reinterpret_cast<slot_ptr>(newBlock + BlockSize - sizeof(slot_type) + 1);
            }
            return reinterpret_cast<pointer>(curSlot++);
        }
    }

    void deallocate(_Tp *p, size_type n=1){
        if (p != nullptr) {
            reinterpret_cast<slot_ptr>(p)->next = freeList;
            freeList = reinterpret_cast<slot_ptr>(p);
        }
    }

    size_type max_size() const noexcept{
        // return std::numeric_limits<size_type>::max() / sizeof(value_type);
        std::size_t sz=2ULL*1024ULL*1024ULL*1024ULL;
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

    // template<typename _Tp>
    // inline bool operator==(const MemPool<_Tp>&, const MemPool<_Tp>&)
    // { return true; }

    // template<typename _Tp>
    // inline bool operator!=(const MemPool<_Tp>&, const MemPool<_Tp>&)
    // { return false; }
};