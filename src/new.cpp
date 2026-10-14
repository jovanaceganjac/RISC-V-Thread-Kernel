#include "../h/syscall_c.hpp"

void* operator new(size_t size) {
    return mem_alloc(size);
}

void* operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete(void* ptr) noexcept {
    mem_free(ptr);
}

void operator delete[](void* ptr) noexcept {
    mem_free(ptr);
}