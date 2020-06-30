#include "mempool.h"
#include <iostream>

// initialize static variables
Block *MemPool::list[LISTNUM] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
PoolHead *MemPool::curpool = nullptr;
char *MemPool::start = nullptr;
char *MemPool::end = nullptr;

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
}

void MemPool::expPool()
{
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
    // get new pool and link it to pool list
    char *newpool = (char *)malloc(sizeof(PoolHead) + BASICPOOLSIZE * sizeof(char));
    ((PoolHead *)newpool)->next = nullptr;
    if (curpool == nullptr)
        curpool = (PoolHead *)newpool;
    else
    {
        curpool->next = (PoolHead *)newpool;
        curpool = curpool->next;
    }
    // reset start and end
    start = newpool + sizeof(PoolHead);
    end = start + BASICPOOLSIZE;
}

void MemPool::refill(size_t index)
{
    size_t n = 16, bytes_left = end - start, bs = blocksize[index];
    // if no even one block in pool, expand pool
    if (bytes_left < bs)
        expPool();
    // else if there are less than n blocks in pool, reset n
    else if (bytes_left < n * bs)
        n = bytes_left / bs;
    // get n blocks to refill list[index]
    for (int i = 0; i < n; i++)
    {
        (*(Block *)start).next = list[index];
        list[index] = (Block *)start;
        start += bs;
    }
}

void *MemPool::mpAlloc(size_t size) noexcept
{
    Block *p;
    // if requested size is too large, just call malloc
    if (size > blocksize[LISTNUM - 1])
    {
        return malloc(size * sizeof(char));
    }
    // find correct list
    int _li = listIndex(size);
    // if list is empty, refill it
    if (list[_li] == nullptr)
        refill(_li);
    // give first free block in list to the caller
    p = list[_li];
    list[_li] = p->next;
    return p;
}

void MemPool::mpFree(void *addr, size_t size) noexcept
{
    // if requested size is too large, just call free
    if (size > MAXBLOCKSIZE)
    {
        free((char *)addr);
        return;
    }
    // link the block to the list of correct size
    size_t _li = listIndex(size);
    ((Block *)addr)->next = list[_li];
    list[_li] = (Block *)addr;
}

size_t MemPool::listIndex(size_t size)
{
    return (size + ALIGN - 1) / ALIGN - 1;
}
