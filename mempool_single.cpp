#include "mempool_single.h"
#include <iostream>

Block *MemPool::list = nullptr;
char *MemPool::start = nullptr;
char *MemPool::end = nullptr;
static int cnt_expPool = 0;
static int cnt_refill = 0;
static int cnt_alloc = 0;
static int cnt_free = 0;

void MemPool::clearPool()
{
    Block *p;
    // for (int i = 0; i < POOLNUM; i++)
    // {
    //     p = list;
    //     while (p != nullptr)
    //     {
    //         free(p);
    //         p = p->next;
    //     }
    // }
    std::cout << cnt_expPool << std::endl
              << cnt_refill << std::endl
              << cnt_alloc << std::endl
              << cnt_free << std::endl;
}

void MemPool::expPool()
{
    cnt_expPool++;
    // allocate rest bytes in pool to freelists
    // if (start != nullptr)
    // {
    //     size_t bytes_left = end - start;
    //     while (bytes_left > 0)
    //     {
    //         (*(Block *)start).next = list;
    //         list = (Block *)start;
    //         start += blocksize;
    //         bytes_left = end - start;
    //     }
    // }
    // get new pool
    start = (char *)malloc(BASICPOOLSIZE * sizeof(char));
    end = start + BASICPOOLSIZE;
}

void *MemPool::refill()
{
    cnt_refill++;
    Block *p;
    size_t n = 4, bytes_left = end - start, bs = blocksize;
    if (bytes_left == 0)
        expPool();
    p = (Block *)start;
    p->next = nullptr;
    start += bs;
    return p;
    // else if (bytes_left < n * bs)
    //     n = bytes_left / bs;
    // for (int i = 0; i < n; i++)
    // {
    //     (*(Block *)start).next = list;
    //     list = (Block *)start;
    //     start += bs;
    // }
    // if (start == end)
    //     expPool();
}

void *MemPool::mpAlloc(size_t size) noexcept
{
    cnt_alloc++;
    Block *p;
    if (size > blocksize)
    {
        return malloc(size * sizeof(char));
    }
    if (list == nullptr)
        p = (Block *)refill();
    else
    {
        p = list;
        list = p->next;
    }
    return p;
}

void MemPool::mpFree(void *addr, size_t size) noexcept
{
    cnt_free++;
    if (size > blocksize)
    {
        free((char *)addr);
        return;
    }
    (*(Block *)addr).next = list;
    list = (Block *)addr;
}

size_t MemPool::listIndex(size_t size)
{
    return (size + ALIGN - 1) / ALIGN - 1;
}
