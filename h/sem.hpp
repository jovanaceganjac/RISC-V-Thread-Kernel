#ifndef PROJECT_BASE_V1_1_SEM_HPP
#define PROJECT_BASE_V1_1_SEM_HPP
#include "syscall_c.hpp"
#include "../lib/hw.h"
#include "scheduler.hpp"
#include "tcb.hpp"

struct WaitingThread {
    TCB* thread;
    unsigned requested;
    int* retVal;
};

class Sem {
public:
    friend class Riscv;

    static int sem_open(sem_t* handle, unsigned init);
    static int sem_close(sem_t handle);
    static int sem_wait(sem_t);
    static int sem_signal(sem_t);
    static int sem_wait_n(sem_t, unsigned n);
    static int sem_signal_n(sem_t, unsigned n);

    void* operator new(size_t size);
    void operator delete(void* ptr);

private:
    explicit Sem(int s=1) : semProm(s), blokirani() {}
    int semProm;
    List<WaitingThread> blokirani;
};


#endif
