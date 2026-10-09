#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 kind; u8 arg; u8 threshold; u8 result; } Rule800A2E48;
typedef struct { u8 pad0[0x50]; s16 delta; s16 pad52; void (*fn)(void *, u8); } Vtbl800A2E48;
typedef struct { u8 type; u8 kind; u8 pad2[0x6]; Vtbl800A2E48 *vtable; } Obj800A2E48;
extern Rule800A2E48 D_8015330C[];
s32 func_8010BEC4(Obj800A2E48 *obj, u8 arg);
/* ODD_C: boolean normalisation helper (precedent func_80100A10.c); expanding `!= 0`
   inside the inline keeps the sne form with $0, which also shapes the final sltu. */
static inline u8 nonzero(u32 value) { return value != 0; }
s32 func_800A2E48(Obj800A2E48 *obj) {
    Rule800A2E48 *rule;
    Vtbl800A2E48 *vt;
    s32 ok;
    for (rule = &D_8015330C[1]; ; rule++) {
        u8 blocked;
        if (rule->arg == 0) {
            break;
        }
        blocked = rule->result;
        if (obj->kind == blocked) {
            return 0;
        }
    }
    for (rule = D_8015330C; ; ) {
        s32 value;
        if (rule->arg == 0) {
            break;
        }
        rule++;
        if (obj->kind == rule->kind) {
            value = (u8)func_8010BEC4(obj, rule->arg);
            if (obj->kind == rule->arg) {
                value--;
            }
            if (value >= rule->threshold) {
                vt = obj->vtable;
                vt->fn((u8 *)obj + vt->delta, rule->result);
                return 1;
            }
        }
    }
    ok = obj->type == 3 && (u8)func_8010BEC4(obj, 1) && (u8)func_8010BEC4(obj, 2) && nonzero((u8)func_8010BEC4(obj, 11));
    if (ok) {
        vt = obj->vtable;
        vt->fn((u8 *)obj + vt->delta, 0x51);
        return 1;
    }
    return 0;
}
