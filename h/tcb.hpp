#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP

#include "../lib/hw.h"
#include "memory_allocator.hpp"

using body=void(*)(void*);

class TCB{
public:
    friend class Sem;
    friend class Riscv;

    static int id;
    static TCB* running;

    static int threadCreate(TCB** handle, body routine, void* arg, void* stack);
    static int threadExit();
    static void dispatch();

    bool getFinished() const { return finished; }
    void setFinished() { finished=true; }
    bool getBlocked() const { return blocked; }
    void setBlocked() { blocked=true; }

    ~TCB() { MemoryAllocator::mem_free(this->stack); }

    void* operator new(size_t size);
    void operator delete(void* ptr);

private:
    struct Context{
        uint64 ra;
        uint64 sp;
    };

    TCB(body routine, void* arg, void * stack);

    uint64 threadID;
    body routine;
    uint64* stack;
    void* arg;
    Context context;
    bool finished;
    bool blocked;

    static void threadWrapper();
    static void contextSwitch(Context* oldContext, Context* newContext);
};

#endif
