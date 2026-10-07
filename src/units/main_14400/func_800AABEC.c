#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x10]; s16 delta; s16 pad12; void (*fn)(void *); } VtblEntry800AABEC;
typedef struct { u8 pad0[0x8]; VtblEntry800AABEC *vtable; } Obj800AABEC;
extern Obj800AABEC *D_80142B10;
void func_800AABEC(void) {
    Obj800AABEC *obj = D_80142B10;
    if (obj != 0) {
        VtblEntry800AABEC *vt = obj->vtable;
        vt->fn((u8 *)obj + vt->delta);
    }
}
