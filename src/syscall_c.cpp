#include "../h/syscall_c.hpp"
#include "../h/memory_allocator.hpp"

void syscall(uint64 op_code, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0, uint64 arg4 = 0) {
    __asm__ volatile("ecall");
}

void* mem_alloc (size_t size) {
    if (size==0) return nullptr;
    void volatile *pov;
    uint64 brBlokova=ROUNDUP(size+MemoryAllocator::BlokSize-1)/MEM_BLOCK_SIZE;
    syscall(MEM_ALLOC, brBlokova);
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return (void*)pov;
}

int mem_free(void* ptr) {
    syscall(MEM_FREE, (uint64)ptr);
    int volatile pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int thread_create(thread_t *handle, void (*routine)(void *), void *arg) {
    void* stack=nullptr;
    if (routine) {
        stack=mem_alloc(DEFAULT_STACK_SIZE);
        if (!stack) return -1;
    }
    syscall(THREAD_CREATE, (uint64)handle, (uint64)routine, (uint64)arg, (uint64)stack);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int thread_exit() {
    syscall(THREAD_EXIT);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

void thread_dispatch() {
    syscall(THREAD_DISPATCH);
}


int sem_open(sem_t* handle, unsigned init) {
    syscall(SEM_OPEN, (uint64)handle, init);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int sem_close(sem_t handle) {
    syscall(SEM_CLOSE, (uint64)handle);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int sem_wait(sem_t id) {
    syscall(SEM_WAIT, (uint64)id);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int sem_signal(sem_t id) {
    syscall(SEM_SIGNAL, (uint64)id);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int sem_wait_n (sem_t id, unsigned n) {
    syscall(SEM_WAIT_N, (uint64)id, n);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int sem_signal_n (sem_t id, unsigned n) {
    syscall(SEM_SIGNAL_N, (uint64)id, n);
    int pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

int time_sleep(time_t sec){
    return 0;
}

char getc() {
    syscall(GETC);
    char pov;
    __asm__ volatile("mv %0, a0" : "=r"(pov));
    return pov;
}

void putc(char c) {
    syscall(PUTC, c);
}
