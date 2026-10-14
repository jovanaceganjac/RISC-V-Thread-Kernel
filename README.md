# RISC-V Thread Kernel

A small multithreaded operating system kernel for RISC-V, written in C++ and assembly
for the *Operating Systems 1* course at ETF Belgrade. It runs on top of a
minimal xv6-based environment in QEMU.

## What it implements

- **Memory allocator** — first-fit allocator with address-sorted free/used lists and
  merging of adjacent free blocks
- **Threads** — thread control blocks (TCB), creation, `exit`, `dispatch`, `sleep`
- **Context switch** in RISC-V assembly and a supervisor trap handler
- **Scheduler** — FIFO ready queue
- **Semaphores** — `open`, `close`, `wait`, `signal`
- **System calls** via `ecall`, with three API layers: ABI (assembly) → C API (`syscall_c`)
  → C++ API (`syscall_cpp`: `Thread`, `Semaphore`, `Console`)

## Structure

```
src/    kernel sources (allocator, TCB, scheduler, semaphores, trap handler, syscalls)
h/      headers
test/   course test programs (producer/consumer, threads, sleep, system mode)
lib/    console and hardware libraries provided by the course
```

## Build and run

Requires the RISC-V GNU toolchain and QEMU (the course environment):

```bash
make qemu
```

The test to run is selected from the console when `userMain` starts.

Based on xv6 (MIT) — see `LICENSE`.
