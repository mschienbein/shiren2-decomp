#include "common.h"
typedef unsigned short u16;
/* Partial object view: the name ID occupies the halfword at offset 0x14. */
typedef struct { unsigned char pad0[0x14]; u16 field_14; } Obj800CECEC;
char *func_80048480(u16 id);
char *func_800CECEC(Obj800CECEC *obj) { return func_80048480(obj->field_14); }
