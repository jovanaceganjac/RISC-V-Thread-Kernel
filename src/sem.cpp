#include "../h/sem.hpp"
#include "../h/riscv.hpp"

void *Sem::operator new(size_t size) {
    size_t brBlokova=ROUNDUP((size+MemoryAllocator::BlokSize))/MEM_BLOCK_SIZE;
    return MemoryAllocator::mem_alloc(brBlokova);
}

void Sem::operator delete(void *ptr) {
    MemoryAllocator::mem_free(ptr);
}

int Sem::sem_open(sem_t *handle, unsigned int init) {
    *handle = new Sem((int)init);
    if (*handle == nullptr){
        return  -1;
    }
    return 0;
}

int Sem::sem_close(sem_t handle) {
    if (!handle) {
        return -1;
    }
    while (handle->blokirani.peek()) {
        WaitingThread* wt = handle->blokirani.get();
        *(wt->retVal) = -1;
        wt->thread->blocked = false;
        Scheduler::put(wt->thread);
        delete wt;
    }
    delete handle;
    return 0;
}

int Sem::sem_wait(sem_t handle) {
    return sem_wait_n(handle, 1);
}

int Sem::sem_signal(sem_t handle) {
    return sem_signal_n(handle, 1);
}

int Sem::sem_wait_n(sem_t handle, unsigned n) {
    if (!handle) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }
    if (!handle->blokirani.peek() && handle->semProm >= (int)n) {
        handle->semProm -= n;
        return 0;
    }
    int retVal = 0;
    WaitingThread* wt = new WaitingThread();
    wt->thread = TCB::running;
    wt->requested = n;
    wt->retVal = &retVal;
    TCB* oldRunning = TCB::running;
    oldRunning->setBlocked();
    handle->blokirani.put(wt);
    TCB::running = Scheduler::get();
    Riscv::push();
    TCB::contextSwitch(&oldRunning->context, &TCB::running->context);
    Riscv::pop();
    return retVal;
}

int Sem::sem_signal_n(sem_t handle, unsigned n) {
    if (!handle) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }
    handle->semProm += n;
    while (handle->blokirani.peek()) {
        WaitingThread* wt = handle->blokirani.peek();
        if (handle->semProm < (int)wt->requested) {
            break;
        }
        wt = handle->blokirani.get();
        handle->semProm -= wt->requested;
        *(wt->retVal) = 0;
        wt->thread->blocked = false;
        Scheduler::put(wt->thread);
        delete wt;
    }
    return 0;
}

/*int Sem::sem_wait(sem_t handle) {
    if (!handle){
        return -1;
    }
    handle->semProm--;
    if (handle->semProm<0) {
        TCB* oldRunning=TCB::running;
        oldRunning->setBlocked();
        handle->blokirani.put(oldRunning);
        TCB::running=Scheduler::get();
        Riscv::push();
        TCB::contextSwitch(&oldRunning->context, &TCB::running->context);
        Riscv::pop();
    }
    return 0;
}

int Sem::sem_signal(sem_t handle) {
    if (!handle){
        return -1;
    }
    handle->semProm++;
    if (handle->semProm <= 0) {
        TCB* thread = handle->blokirani.get();
        thread->blocked=false;
        Scheduler::put(thread);
    }
    return 0;
}

int Sem::sem_close(sem_t handle) {
    if (!handle){
        return -1;
    }
    while (handle->blokirani.peek()) {
        TCB* thread = handle->blokirani.get();
        thread->blocked=false;
        Scheduler::put(thread);
    }
    delete handle;
    return 0;
}

*/