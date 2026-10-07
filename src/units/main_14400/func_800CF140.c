#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 kind; u8 pad1; u8 flags; } Entry800CF140;
typedef struct { s32 data[4]; } Iter800CF140;
Iter800CF140 *func_800CEB20(Iter800CF140 *it, void *list);
s32 func_800CEBA0(Iter800CF140 *it);
Entry800CF140 *func_800CEC68(Iter800CF140 *it);

s32 func_800CF140(void *list, u8 kind) {
    Iter800CF140 it;
    s32 count = 0;

    func_800CEB20(&it, list);
    while (func_800CEBA0(&it)) {
        Entry800CF140 *e = func_800CEC68(&it);
        s32 match = 0;
        if (e->flags & 4) {
            match = e->kind == kind;
        }
        if (match) {
            count++;
        }
    }
    return count;
}
