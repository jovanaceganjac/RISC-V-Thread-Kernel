#include "../h/scheduler.hpp"

List<TCB> Scheduler::ready;

TCB* Scheduler::get(){
    return ready.get();
}

void Scheduler::put(TCB *thread){
    ready.put(thread);
}

void *Scheduler::operator new(size_t size) {
    size_t brBlokova=ROUNDUP((size+MemoryAllocator::BlokSize))/MEM_BLOCK_SIZE;
    return MemoryAllocator::mem_alloc(brBlokova);
}

void Scheduler::operator delete(void *ptr) {
    MemoryAllocator::mem_free(ptr);
}