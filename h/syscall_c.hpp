#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP
#include "../lib/hw.h"

constexpr uint64 MEM_ALLOC=0x01;
constexpr uint64 MEM_FREE=0x02;
constexpr uint64 THREAD_CREATE=0x11;
constexpr uint64 THREAD_EXIT=0x12;
constexpr uint64 THREAD_DISPATCH=0x13;
constexpr uint64 SEM_OPEN=0x21;
constexpr uint64 SEM_CLOSE=0x22;
constexpr uint64 SEM_WAIT=0x23;
constexpr uint64 SEM_SIGNAL=0x24;
constexpr uint64 SEM_WAIT_N=0x25;
constexpr uint64 SEM_SIGNAL_N=0x26;
constexpr uint64 GETC=0x41;
constexpr uint64 PUTC=0x42;

class Sem;
class TCB;
typedef TCB* thread_t;
typedef Sem* sem_t;
typedef unsigned long time_t;

void* mem_alloc (size_t size);
int mem_free (void*);


int thread_create (thread_t* handle, void(*body)(void*), void* arg);
int thread_exit();
void thread_dispatch ();


int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);
int sem_wait_n (sem_t id, unsigned n);
int sem_signal_n (sem_t id, unsigned n);
int time_sleep(time_t);


char getc();
void putc(char);

#endif
