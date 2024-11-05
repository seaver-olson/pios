#ifndef __RPRINTF_H__
#define __RPRINTF_H__


//#include <ctype.h>
//#include <string.h>
#include <stdarg.h>
#include "color.h"

typedef unsigned int  size_t;

#define NULL (void*)0

void putc(int data);
int getc();
int strcmp(char *first, char *second);
void *memcpy(void *dest, const void *src, size_t n);
int strncmp(char *first, char *second, size_t n);
size_t strlen(const char *str);

int isdig(int c); // hand-implemented alternative to isdigit(), which uses a bunch of c library functions I don't want to include.

typedef char *charptr;
typedef void (*func_ptr)(int);

///////////////////////////////////////////////////////////////////////////////
////  Common Prototype functions
/////////////////////////////////////////////////////////////////////////////////
void esp_sprintf(char *buf, char *ctrl, ...);
void esp_vprintf( const func_ptr f_ptr, charptr ctrl, va_list argp);
void esp_printf( const func_ptr f_ptr, charptr ctrl, ...);
void esp_printhex(unsigned int num);
void printk(charptr ctrl, ...);
size_t strlen(const char *c);
int tolower(int c);
void success(char *str);
void fail(char *str);
void warning(char *str);
#endif
