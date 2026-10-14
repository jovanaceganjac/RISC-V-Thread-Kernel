#include "../lib/hw.h"
#include "../h/memory_allocator.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"

thread_t mainThread;
thread_t userMainThread;

extern void printInt(int integer);
extern void printStr(char const* string);
extern void userMain();

void userWrapper(void*) {
    userMain();
}

int main(){

    Riscv::writeStvec((uint64)&Riscv::supervisorTrap);
    MemoryAllocator::init();

    thread_create(&mainThread, nullptr, nullptr);
    TCB::running = mainThread;

    thread_create(&userMainThread, userWrapper, nullptr);

    while (!userMainThread->getFinished()) thread_dispatch();

    return 0;
}
