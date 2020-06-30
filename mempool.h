#pragma once
#include <climits>
#include <cstddef>
#include <iostream>

#define BASICPOOLSIZE 65536 // The number of bytes requested each time a new pool is requested
#define LISTNUM 16 // Number of freelists of different sizes
#define ALIGN 16 // Difference in adjacent block sizes
#define MAXBLOCKSIZE (LISTNUM * ALIGN) // = blocksize[LISTNUM - 1]

// different size of blocks
const size_t blocksize[LISTNUM] = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256};

// A Block is a node in freelist
union Block
{
    union Block *next;
    char data[1];
};

// PoolHead constitutes a linked list of pools that have been applied for
typedef struct _PoolHead
{
    struct _PoolHead *next;
} PoolHead;

class MemPool
{
public:
    // memory pool allocation (if >MAXBLOCKSIZE, just new)
    static void *mpAlloc(size_t size) noexcept;
    // return memory to freelist
    static void mpFree(void *addr, size_t size) noexcept;
    // free all memory in all pools
    static void clearPool();

private:
    // unused
    MemPool() {}
    // if current pool is empty, get new pool (expand pool)
    static void expPool();
    // find index of correct blocksize
    static size_t listIndex(size_t size);
    // if list is empty, refill it
    static void refill(size_t index);

private:
    static Block *list[LISTNUM];
    static PoolHead *curpool;
    static char *start; // start address of current pool
    static char *end; // end address of current pool
};