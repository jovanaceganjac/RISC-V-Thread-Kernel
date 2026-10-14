#ifndef PROJECT_BASE_V1_1_SYSCALLCPP_HPP
#define PROJECT_BASE_V1_1_SYSCALLCPP_HPP

#include "syscall_c.hpp"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread();
    int start();
    static void dispatch ();

protected:
    Thread ();
    virtual void run () {};

private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;

    static void wrapper(void*);
};


class Semaphore {
public:
    Semaphore(unsigned init=1);
    virtual ~Semaphore();
    int wait();
    int signal();

private:
    sem_t myHandle;
};


class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif
