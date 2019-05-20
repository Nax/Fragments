#ifndef _STDDEF_H
#define _STDDEF_H 1

#include <sys/_null.h>
#include <sys/_size_t.h>

#define offsetof(st, m) ((size_t)(&(((st*)0)->m)))

#endif
