#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include "my_allocator.hpp"
using namespace std;

// include header of your allocator here
template <class T>
using StdAllocator = allocator<T>; // replace the std::allocator with your allocator
template<class T>
using MyAllocator = MemPool<T>;
using Point2D = std::pair<int, int>;

const int TestSize = 10000;
const int PickSize = 1000;

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, TestSize);

    //std::alloc
    clock_t start=clock();
    // vector creation
    using IntVec1 = std::vector<int, StdAllocator<int>>;
    std::vector<IntVec1, StdAllocator<IntVec1>> vecints1(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecints1[i].resize(dis(gen));

    using PointVec1 = std::vector<Point2D, StdAllocator<Point2D>>;
    std::vector<PointVec1, StdAllocator<PointVec1>> vecpts1(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts1[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints1[idx].resize(size);
        vecpts1[idx].resize(size);
    }
    clock_t end=clock();
    cout << "std::alloctor time: " << (double)(end - start)*1000.0 / CLOCKS_PER_SEC << endl;

    // vector element assignment
    // {
    //     int val = 10;
    //     int idx1 = dis(gen) - 1;
    //     int idx2 = vecints[idx1].size() / 2;
    //     vecints[idx1][idx2] = val;
    //     if (vecints[idx1][idx2] == val)
    //         std::cout << "correct assignment in vecints: " << idx1 << std::endl;
    //     else
    //         std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    // }
    // {
    //     Point2D val(11, 15);
    //     int idx1 = dis(gen) - 1;
    //     int idx2 = vecpts[idx1].size() / 2;
    //     vecpts[idx1][idx2] = val;
    //     if (vecpts[idx1][idx2] == val)
    //         std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
    //     else
    //         std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    // }

    //my_alloc
    start=clock();
    // vector creation
    using IntVec2 = std::vector<int, MyAllocator<int>>;
    std::vector<IntVec2, MyAllocator<IntVec2>> vecints2(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecints2[i].resize(dis(gen));

    using PointVec2 = std::vector<Point2D, MyAllocator<Point2D>>;
    std::vector<PointVec2, MyAllocator<PointVec2>> vecpts2(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts2[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints2[idx].resize(size);
        vecpts2[idx].resize(size);
    }
    end=clock();
    cout << "std::alloctor time: " << ((double)(end - start)*1000.0 / CLOCKS_PER_SEC) << endl;
    return 0;
}
