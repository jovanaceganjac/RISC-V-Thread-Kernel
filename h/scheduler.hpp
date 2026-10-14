#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

#include "tcb.hpp"
#include "list.hpp"

class Scheduler{
public:
    friend class Riscv;

    static TCB* get();
    static void put(TCB* thread);

    void* operator new(size_t);
    void operator delete(void *);
    Scheduler(const Scheduler&)=delete;

private:
    Scheduler()=default;
    static List<TCB> ready;
};

#endif
