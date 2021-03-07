#pragma once
#include <cstddef>
#include <iostream>

#define BASICPOOLSIZE 65536
#define POOLNUM 16
#define ALIGN 16
#define MAXBLOCKSIZE (POOLNUM * ALIGN)

// different size of blocks
const size_t blocksize = 4096;

union Block
{
    union Block *next;
    char *data;
};

class MemPool
{
public:
    // memory pool allocation (if >128, just new)
    static void *mpAlloc(size_t size) noexcept;
    static void mpFree(void *addr, size_t size) noexcept;
    static void clearPool();

private:
    // unused
    MemPool() {}
    // if any pool is empty, get new memory sapce
    static void expPool();
    // find index of correct blocksize
    static size_t listIndex(size_t size);
    // if list is empty, refill it
    static void* refill();
    static size_t ROUND_UP(size_t bytes);

private:
    static Block *list; // 8, 16, 32, 64, 128 (Bytes)
    static char *start;
    static char *end;
};