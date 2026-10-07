#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
/* Widget vtable slot 13 entry (+0x68 this-adjust, +0x6C method): value of a flattened
 * item index, 0x80000000 = none. */
typedef struct { s16 delta; s16 index; s32 (*fn)(void *self, s32 index); } ValueSlot;
typedef struct { u8 pad[0x68]; ValueSlot value; } VTable;
typedef struct { u8 pad[0x4C]; VTable *vtbl; u8 pad50[0x24]; u8 ids[0x1C]; s32 x90; } S;
u16 func_80112DC4(u8);
char *func_80048480(u16);
char *func_80083C90(char *, char *);
/* Widget vtable slot 11 (+0x58/+0x5C): writes the text of item idx into out. */
void func_8009E0D4(S *s, s32 idx, char *out) {
    s32 msg;
    ValueSlot *e;
    char *str = out;
    *str = 0;
    if (s->x90 == 0) {
        msg = 0x253;
    } else {
        e = &s->vtbl->value;
        if (e->fn((u8 *)s + e->delta, idx) == 0x80000000) return;
        msg = func_80112DC4(s->ids[idx]);
    }
    func_80083C90(str, func_80048480(msg));
}
