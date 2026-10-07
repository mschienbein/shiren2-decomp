#include "common.h"

typedef struct { s32 data[4]; } Entry;
typedef struct { char pad[0x2DC]; Entry entries[16]; s32 count3DC; } Obj;
s32 func_800A32D8(void *a, void *b);
s32 func_800B9FE0(Obj *obj, void *rect) {
    s32 i = 0;
next:
    if (i < obj->count3DC) {
        /* local-arithmetic-qualification: entry i has stride 16 at obj+0x2DC;
         * pointer arithmetic reverses the original addu a1,a1,s1 operands. */
        if (func_800A32D8(rect, (void *)(i * 16 + (u32)obj + 0x2DC))) {
            return 1;
        }
        i++;
        goto next;
    }
    return 0;
}
