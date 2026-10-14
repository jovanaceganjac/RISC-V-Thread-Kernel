#ifndef PROJECT_BASE_V1_1_RISCV_HPP
#define PROJECT_BASE_V1_1_RISCV_HPP

#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/syscall_c.hpp"
#include "../h/memory_allocator.hpp"
#include "../test/printing.hpp"

constexpr uint64 SOFTWARE = 0x8000000000000001UL;
constexpr uint64 EXTERNAL = 0x8000000000000009UL;
constexpr uint64 U_ECALL  = 0x8UL;
constexpr uint64 S_ECALL  = 0x9UL;

class Riscv{
public:
    static void supervisorTrap();
    static void interruptHandler();
    static void push();
    static void pop();
    static void popSppSpie();
    static uint64 readScause();
    static void writeScause(uint64 scause);
    static uint64 readSepc();
    static void writeSepc(uint64 sepc);
    static uint64 readStvec();
    static void writeStvec(uint64 stvec);

    enum bitmaskSip{
        SSIP = (1 << 1),
        SEIP = (1 << 9)
    };

    static void setSip(uint64 mask);
    static void clearSip(uint64 mask);
    static uint64 readSip();
    static void writeSip(uint64 sip);

    enum bitmaskSstatus{
        SIE = (1 << 1),
        SPIE = (1 << 5),
        SPP = (1 << 8)
    };

    static void setSstatus(uint64 mask);
    static void clearSstatus(uint64 mask);
    static uint64 readSstatus();
    static void writeSstatus(uint64 sstatus);
};

inline uint64 Riscv::readScause() {
    uint64 volatile value;
    __asm__ volatile ("csrr %0, scause" : "=r"(value));
    return value;
}

inline void Riscv::writeScause(uint64 value) {
    __asm__ volatile ("csrw scause, %0" : : "r"(value));
}

inline uint64 Riscv::readSepc() {
    uint64 volatile value;
    __asm__ volatile ("csrr %0, sepc" : "=r"(value));
    return value;
}

inline void Riscv::writeSepc(uint64 value) {
    __asm__ volatile ("csrw sepc, %0" : : "r"(value));
}

inline uint64 Riscv::readStvec() {
    uint64 volatile value;
    __asm__ volatile ("csrr %0, stvec" : "=r"(value));
    return value;
}

inline void Riscv::writeStvec(uint64 value) {
    __asm__ volatile ("csrw stvec, %0" : : "r"(value));
}

inline void Riscv::setSip(uint64 mask) {
    __asm__ volatile("csrs sip, %0" : : "r"(mask));
}

inline void Riscv::clearSip(uint64 mask) {
    __asm__ volatile("csrc sip, %0" : : "r"(mask));
}

inline uint64 Riscv::readSip() {
    uint64 volatile value;
    __asm__ volatile ("csrr %0, sip" : "=r"(value));
    return value;
}

inline void Riscv::writeSip(uint64 value) {
    __asm__ volatile ("csrw sip, %0" : : "r"(value));
}

inline void Riscv::setSstatus(uint64 mask) {
    __asm__ volatile("csrs sstatus, %0" : : "r"(mask));
}

inline void Riscv::clearSstatus(uint64 mask) {
    __asm__ volatile("csrc sstatus, %0" : : "r"(mask));
};

inline uint64 Riscv::readSstatus() {
    uint64 volatile value;
    __asm__ volatile ("csrr %0, sstatus" : "=r"(value));
    return value;
}

inline void Riscv::writeSstatus(uint64 value) {
    __asm__ volatile ("csrw sstatus, %0" : : "r"(value));
}

#endif
