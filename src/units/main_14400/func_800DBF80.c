#include "common.h"

typedef unsigned char u8;
typedef struct Reference {
    void *container;
    void *entry;
} Reference;
typedef struct Object {
    u8 pad_00[0x10];
    Reference reference_10;
} Object;
extern s32 func_800DADCC(unsigned char *object);
extern s32 func_800D0248(void *reference);

s32 func_800DBF80(Object *object)
{
    s32 invalid = func_800DADCC((u8 *)object) != 1;
    if (invalid)
        return 0;
    return func_800D0248(&object->reference_10);
}
