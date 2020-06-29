#include "mempool.h"
#include <iostream>

Block *MemPool::list[POOLNUM] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
PoolHead *MemPool::curpool = nullptr;
char *MemPool::start = nullptr;
char *MemPool::end = nullptr;
static int cnt_expPool = 0;
static int cnt_refill = 0;
static int cnt_alloc = 0;
static int cnt_free = 0;

void MemPool::clearPool()
{
    PoolHead *p = curpool;
    PoolHead *tmp;
    while (p != nullptr)
    {
        tmp = p;
        p = p->next;
        delete (char *)tmp;
    }
    std::cout << cnt_expPool << std::endl
              << cnt_refill << std::endl
              << cnt_alloc << std::endl
              << cnt_free << std::endl;
}

void MemPool::expPool()
{
    cnt_expPool++;
    // allocate rest bytes in pool to freelists
    if (start != nullptr)
    {
        size_t bytes_left = end - start;
        while (bytes_left > 0)
        {
            size_t _li = listIndex(bytes_left) - 1;
            (*(Block *)start).next = list[_li];
            list[_li] = (Block *)start;
            start += blocksize[_li];
            bytes_left = end - start;
        }
    }
    // get new pool
    char *newpool = (char *)malloc(sizeof(PoolHead) + BASICPOOLSIZE * sizeof(char));
    ((PoolHead *)newpool)->next = nullptr;
    if (curpool == nullptr)
        curpool = (PoolHead *)newpool;
    else
    {
        curpool->next = (PoolHead *)newpool;
        curpool = curpool->next;
    }
    start = newpool + sizeof(PoolHead);
    end = start + BASICPOOLSIZE;
}

void MemPool::refill(size_t index)
{
    cnt_refill++;
    size_t n = 16, bytes_left = end - start, bs = blocksize[index];
    if (bytes_left < bs)
        expPool();
    else if (bytes_left < n * bs)
        n = bytes_left / bs;
    for (int i = 0; i < n; i++)
    {
        (*(Block *)start).next = list[index];
        list[index] = (Block *)start;
        start += bs;
    }
}

void *MemPool::mpAlloc(size_t size) noexcept
{
    cnt_alloc++;
    Block *p;
    if (size > blocksize[POOLNUM - 1])
    {
        return malloc(size * sizeof(char));
    }

    int i = listIndex(size);

    if (list[i] == nullptr)
        refill(i);

    p = list[i];
    list[i] = p->next;
    return p;
}

void MemPool::mpFree(void *addr, size_t size) noexcept
{
    cnt_free++;
    if (size > MAXBLOCKSIZE)
    {
        free((char *)addr);
        return;
    }
    size_t _li = listIndex(size);
    (*(Block *)addr).next = list[_li];
    list[_li] = (Block *)addr;
}

size_t MemPool::listIndex(size_t size)
{
    return (size + ALIGN - 1) / ALIGN - 1;
}
