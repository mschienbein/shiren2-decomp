#include "common.h"
typedef unsigned short u16;
typedef struct Obj800EB7C4 Obj800EB7C4;
typedef struct Entry800EB7C4 Entry800EB7C4;
extern s32 D_80148364, D_801C9F18;
extern u16 D_8015693C, D_8015693A;
extern Entry800EB7C4 *func_800EB7C4(Obj800EB7C4 *object);
extern u16 func_800AE710(Entry800EB7C4 *object);
s32 func_800EB820(Obj800EB7C4 *object) {
    Entry800EB7C4 *entry;
    s32 value;
    if (D_80148364 == 0) {
        D_801C9F18 = (u32)D_8015693C / (u32)D_8015693A;
        D_80148364 = 1;
    }
    entry = func_800EB7C4(object);
    if (!entry) return 0;
    value = func_800AE710(entry);
    if (value > D_801C9F18) value = D_801C9F18;
    return value;
}
