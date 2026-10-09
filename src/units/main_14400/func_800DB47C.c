#include "common.h"
typedef unsigned char u8;
/* Second container/element link at +0x10, validated by func_800D0248. */
typedef struct { void *container; void *element; } Link;
typedef struct { u8 pad0[0x10]; Link link_10; } Object;
extern s32 func_800DADCC(u8 *object);
extern s32 func_800D0248(void *link);
s32 func_800DB47C(Object *object) {
    s32 failed = func_800DADCC((u8 *)object) != 1;
    if (failed) return 0;
    return func_800D0248(&object->link_10);
}
