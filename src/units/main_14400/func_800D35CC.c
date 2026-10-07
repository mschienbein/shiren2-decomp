#include "common.h"
typedef struct { s32 field0; void *field4; } Object;
extern s32 func_800A31C8(void *rect, void *pos);
extern s32 func_800D33FC(Object *);
extern s32 func_800D3510(Object *);
s32 func_800D35CC(Object *p, void *first, void *second) {
    s32 old = func_800A31C8(p->field4, first);
    s32 now = func_800A31C8(p->field4, second);
    if (!old) {
        if (now) return func_800D33FC(p);
    } else {
        if (!now) return func_800D3510(p);
    }
    return 0;
}
