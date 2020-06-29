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
    // memory pool allocation (if >128, just new)
    static void *mpAlloc(size_t size) noexcept;
    static void mpFree(void *addr) noexcept;

private:
    // unused
    MemPool() {}
    // initialize all pools
    void initPools();
    // if any pool is empty, get new memory sapce
    static void expPool(size_t index);

private:
    static char *pool[POOLNUM]; // 8, 16, 32, 64, 128 (Bytes)
};