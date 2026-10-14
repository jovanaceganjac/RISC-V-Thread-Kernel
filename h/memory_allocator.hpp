#ifndef PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_HPP
#include "../lib/hw.h"
#include "../lib/console.h"

#define ROUNDUP(size) (((size)+MEM_BLOCK_SIZE-1) & ~(MEM_BLOCK_SIZE-1))

class MemoryAllocator{
public:
    static const size_t BlokSize;

    static void init();
    static void* mem_alloc(size_t size);
    static int mem_free (void* ptr);

    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;

private:
    struct Blok{
        size_t size;
        Blok* sled;
        Blok* pret;
    };
    MemoryAllocator()=default;

    static Blok* head_zauz;
    static Blok* head_slob;
    static bool initialized;

    static void tryToJoin(Blok* curr);
};

#endif
