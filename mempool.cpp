#include "mempool.h"
#include "assert.h"

void MemPool::initPools()
{
    int i = 0;
    for (i = 0; i < POOLNUM; i++)
    {
        // get huge space from memory
        pool[i] = new char[BASICPOOLSIZE + (sizeof(char *) + sizeof(unsigned char)) * BASICPOOLSIZE / blocksize[i]];
        // init freelist for all pools
        int j;
        char **p1;
        unsigned char *p2;
        for (j = 0; j < BASICPOOLSIZE / blocksize[i] - 1; j++)
        {
            p1 = (char **)&pool[i][j * (blocksize[i] + sizeof(char *) + sizeof(unsigned char))];
            *p1 = &pool[i][(j + 1) * (blocksize[i] + sizeof(char *) + sizeof(unsigned char))];
            p2 = (unsigned char *)&pool[i][j * (blocksize[i] + sizeof(char *) + sizeof(unsigned char)) + sizeof(char *)];
            *p2 = (unsigned char)i;
        }
        p1 = (char **)&pool[i][j * (blocksize[i] + sizeof(char *) + sizeof(unsigned char))];
        *p1 = nullptr;
    }
}

void MemPool::expPool(size_t index)
{
    // get huge space from memory
    pool[index] = new char[BASICPOOLSIZE + 9 * BASICPOOLSIZE / blocksize[index]];
    // init freelist for all pools
    int j;
    char **p1;
    unsigned char *p2;
    for (j = 0; j < BASICPOOLSIZE / blocksize[index] - 1; j++)
    {
        p1 = (char **)&pool[index][j * (blocksize[index] + sizeof(char *) + sizeof(unsigned char))];
        *p1 = &pool[index][(j + 1) * (blocksize[index] + sizeof(char *) + sizeof(unsigned char))];
        p2 = (unsigned char *)&pool[index][j * (blocksize[index] + sizeof(char *) + sizeof(unsigned char)) + sizeof(char *)];
        *p2 = (unsigned char)index;
    }
    p1 = (char **)&pool[index][j * (blocksize[index] + sizeof(char *) + sizeof(unsigned char))];
    *p1 = nullptr;
    p2 = (unsigned char *)&pool[index][j * (blocksize[index] + sizeof(char *) + sizeof(unsigned char)) + sizeof(char *)];
    *p2 = (unsigned char)index;
}

void *MemPool::mpAlloc(size_t size) noexcept
{
    int i;
    for (i = 0; i < POOLNUM; i++)
    {
        if (blocksize[i] >= size)
            break;
    }

    if (pool[i] == nullptr)
        expPool(i);

    char *cp;
    char **cpp;
    cp = &pool[i][sizeof(char *) + sizeof(unsigned char)];
    cpp = (char **)pool[i];
    pool[i] = *cpp;
    return cp;
}

void MemPool::mpFree(void *addr) noexcept
{
    unsigned char *ucp = (unsigned char *)((char *)addr - sizeof(unsigned char));
    char **cpp = (char **)((char *)addr - sizeof(unsigned char) - sizeof(char *));
    int i = *ucp;
    char *p = pool[i];
    *cpp = p;
    pool[i] = (char *)cpp;
}