#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/sem.hpp"

extern void printString(char const* string);

void Riscv::interruptHandler(){
    uint64 scause = readScause();
    if(scause==EXTERNAL){
        console_handler();
    }
    else if(scause==SOFTWARE) {
        clearSip(bitmaskSip::SSIP);
    }
    else if(scause==U_ECALL || scause==S_ECALL){
        uint64 volatile oc, arg1, arg2, arg3, arg4;
        uint64 sepc=readSepc()+4;
        uint64 sstatus=readSstatus();

        __asm__ volatile("mv %0, a0" : "=r"(oc));
        switch(oc){
            case MEM_ALLOC:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                MemoryAllocator::mem_alloc((size_t)arg1);
                break;
            case MEM_FREE:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                MemoryAllocator::mem_free((void *)arg1);
                break;
            case THREAD_CREATE:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                __asm__ volatile("mv %0, a2" : "=r"(arg2));
                __asm__ volatile("mv %0, a3" : "=r"(arg3));
                __asm__ volatile("mv %0, a4" : "=r"(arg4));
                TCB::threadCreate((thread_t *)arg1, (body)arg2, (void *)arg3, (void *)arg4);
                break;
            case THREAD_EXIT:
                TCB::threadExit();
                break;
            case THREAD_DISPATCH:
                TCB::dispatch();
                break;
            case SEM_OPEN:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                __asm__ volatile("mv %0, a2" : "=r"(arg2));
                Sem::sem_open((sem_t*)arg1, (unsigned int)arg2);
                break;
            case SEM_CLOSE:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                Sem::sem_close((sem_t)arg1);
                break;
            case SEM_WAIT:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                Sem::sem_wait((sem_t)arg1);
                break;
            case SEM_SIGNAL:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                Sem::sem_signal((sem_t)arg1);
                break;
            case SEM_WAIT_N:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                __asm__ volatile("mv %0, a2" : "=r"(arg2));
                Sem::sem_wait_n((sem_t)arg1, (unsigned int)arg2);
                break;
            case SEM_SIGNAL_N:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                __asm__ volatile("mv %0, a2" : "=r"(arg2));
                Sem::sem_signal_n((sem_t)arg1, (unsigned int)arg2);
                break;
            case GETC:
                __getc();
                break;
            case PUTC:
                __asm__ volatile("mv %0, a1" : "=r"(arg1));
                __putc(arg1);
                break;
        }
        writeSepc(sepc);
        writeSstatus(sstatus);
    }
    else{
        printString("Error");
    }
}

void Riscv::popSppSpie(){
    __asm__ volatile ("csrw sepc, ra");
    Riscv::clearSstatus(bitmaskSstatus::SPP);
    __asm__ volatile ("sret");
}