#include "common.h"

typedef unsigned char u8;
typedef struct { s32 field00; u8 pad04[0x1C]; } Record;
typedef struct { u8 pad00[0x50]; Record *records; } Object;

s32 func_8009C9A0(Object *object, s32 index)
{
    return object->records[index / 3].field00;
}
