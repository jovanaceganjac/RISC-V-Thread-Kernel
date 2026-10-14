#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

Thread::Thread(void (*body)(void*), void *arg) {
    this->myHandle=nullptr;
    this->body=body;
    this->arg=arg;
}

Thread::Thread() {
    this->myHandle=nullptr;
    this->body=wrapper;
    this->arg=(void*)this;
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    return thread_create(&myHandle, this->body, this->arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::wrapper(void *arg) {
    Thread* thread=(Thread*)arg;
    thread->run();
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}