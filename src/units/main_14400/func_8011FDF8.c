#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
/* Trap +0x44 supplies self; this constant predicate does not read it. */
s32 func_8011FDF8(void *self) { return 1; }
