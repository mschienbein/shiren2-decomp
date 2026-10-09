#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct S S;

extern void *func_800A38FC(s32 size);
extern S *func_80101BC0(S *p, u8 a);

/* Factory slot of D_8015CC64 (dispatched by func_800A8694): construct in place when
 * storage is supplied, otherwise allocate it. */
void *func_80101B80(u8 kind, void *storage) {
    if (storage != 0) {
        return func_80101BC0(storage, kind);
    }
    return func_80101BC0(func_800A38FC(0xA0), kind);
}
