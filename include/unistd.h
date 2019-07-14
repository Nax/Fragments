#ifndef _UNISTD_H
#define _UNISTD_H 1

#include <stdint.h>

#include <sys/__begin.h>
uintptr_t syscall(uintptr_t sysno, uintptr_t a0, uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4, uintptr_t a5);
#include <sys/__end.h>

#endif
