#include <cassert>	// assert()
#include <ctime>	// clock()
#include <iostream> // std::cout, std::endl
#include <vector>	// std::vector
#include <list>

#include "allocator.h"
// #include "allocator_std.h"
#include "mempool.h" // MemoryPool<T>

// 插入元素个数
#define ELEMS 10000
// 重复次数
#define REPS 1000

int main()
{
	clock_t start, end;

	// 使用 STL 默认分配器
	std::list<int, std::allocator<int>> stackDefault;
	start = clock();
	for (int j = 0; j < REPS; j++)
	{
		assert(stackDefault.empty());
		for (int i = 0; i < ELEMS; i++)
		{
			stackDefault.push_back(i);
		}
		
		for (int i = 0; i < ELEMS; i++)
		{
			stackDefault.pop_back();
		}
	}
	std::cout << "Default Allocator Time: ";
	std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

	// 使用内存池
	std::list<int, my::allocator<int>> stackPool;
	start = clock();
	for (int j = 0; j < REPS; j++)
	{
		assert(stackPool.empty());
		for (int i = 0; i < ELEMS; i++)
		{
			stackPool.push_back(i);
		}
		
		for (int i = 0; i < ELEMS; i++)
		{
			stackPool.pop_back();
		}
	}
	std::cout << "MemoryPool Allocator Time: ";
	std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

	MemPool::clearPool();

	return 0;
}