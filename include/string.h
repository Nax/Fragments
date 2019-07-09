#ifndef _STRING_H
#define _STRING_H 1

#include <sys/_null.h>
#include <sys/_size_t.h>

#include <sys/__begin.h>
void* memset(void* dest, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
#include <sys/__end.h>

#endif
