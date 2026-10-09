#include "common.h"
/* The display is a complete 0x10-byte subobject, including its handle at +0xC. */
typedef struct { unsigned char pad00[0x54]; unsigned char display54[0x10]; } Object;
extern char *func_80048480(unsigned short);
extern void func_800487EC(void *, s32, s32, const char *);
void func_8009E208(Object *self) { char *value = func_80048480(0x254); func_800487EC(self->display54, 0, 0, value); }
