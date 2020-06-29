#pragma once
#include <climits>
#include <cstddef>
#include <iostream>

#define BASICPOOLSIZE 65536
#define POOLNUM 16
#define ALIGN 16
#define MAXBLOCKSIZE (POOLNUM * ALIGN)

// different size of blocks
const size_t blocksize[POOLNUM] = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256};

union Block
{
    union Block *next;
    char data[1];
};

typedef struct _PoolHead
{
    struct _PoolHead *next;
} PoolHead;

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
    // if current pool is empty, get new pool
    static void expPool();
    // find index of correct blocksize
    static size_t listIndex(size_t size);
    // if list is empty, refill it
    static void refill(size_t index);
    static size_t ROUND_UP(size_t bytes);

private:
    static Block *list[POOLNUM]; // 8, 16, 32, 64, 128 (Bytes)
    static PoolHead *curpool;
    static char *start;
    static char *end;
};