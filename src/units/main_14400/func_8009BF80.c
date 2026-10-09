#include "common.h"

typedef unsigned char u8;
typedef struct Obj {
    u8 pad_00[0x70];
    u8 characters_70[8];
    s32 index_78;
} Obj;
extern u8 func_8009BFFC(u8 character, s32 operation);

s32 func_8009BF80(Obj *object, s32 operation)
{
    s32 index = object->index_78;
    u8 previous = object->characters_70[index];
    if (!previous) {
        if (index == 0)
            return 0;
        --index;
        previous = object->characters_70[index];
    }
    object->characters_70[index] = func_8009BFFC(previous, operation);
    return object->characters_70[index] != previous;
}
