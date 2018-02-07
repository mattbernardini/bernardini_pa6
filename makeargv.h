#ifndef ERRNO_H
#define ERRNO_H
#include <errno.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

int makeargv(const char *s, const char *delimiters, char ***argvp);
