#include "common.h"
/* Second container/element link at +0x10, validated by func_800D0248. */
typedef struct { void *container; void *element; } Link;
typedef struct { unsigned char pad_00[0x10]; Link link_10; } Object;
extern s32 func_800DADCC(unsigned char *object);
extern s32 func_800D0248(void *link);
s32 func_800DC6BC(Object *object) {
    if ((func_800DADCC((unsigned char *)object) ^ 1) == 0) return func_800D0248(&object->link_10);
    return 0;
}
