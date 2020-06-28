#include <climits>
#include <cstddef>
#include <iostream>

#define BASICPOOLSIZE 4096
#define POOLNUM 5

// different size of blocks
const size_t blocksize[POOLNUM] = {8, 16, 32, 64, 128};

class MemPool
{
public:
    // initialize all pools
    void initPools();
    // memory pool allocation (if >128, just new)
    void *mpAlloc(size_t size);

private:
    // unused
    MemPool() {}
    // if any pool is empty, get new memory sapce
    void expPool(size_t blocksize);
    // BLOCK DATA or FREELIST POINTER
    union Block
    {
        char data;
        Block *nextfree;
    };

private:
    static Block *pool[POOLNUM]; // 8, 16, 32, 64, 128 (Bytes)
};