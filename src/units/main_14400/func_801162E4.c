#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 pad0 : 8;
    u32 enabled : 1;
    u32 pad9 : 23;
} WordFlags;

typedef struct {
    char pad0[0x20];
    WordFlags flags;
} State;

typedef struct {
    char pad0[0xC];
    u8 flags;
} Obj;

extern State *D_801476B8;
extern u8 D_80142F24;

s32 func_801162E4(Obj *obj, s32 kind) {
    if (kind == 0x23) {
        s32 result = 0;
        /* Stack copy of the state flags; kept in memory through `p`, never read back. */
        WordFlags flags;
        WordFlags *p = &flags;

        if ((obj->flags >> 1) & 1) {
            result = 1;
        } else {
            State *s = D_801476B8;
            u32 enabled = s->flags.enabled;

            *p = s->flags;
            if (!enabled) {
                result = 1;
            } else if ((obj->flags & 1) && D_80142F24 != 10) {
                result = 1;
            }
        }
        return result;
    } else {
        s32 result = 0;

        if (kind == 10 || kind == 0x24 || kind == 11) {
            result = 1;
        }
        return result;
    }
}
