#include "common.h"

typedef struct { s32 index; } Iter;
typedef struct { unsigned char bytes[0xE4]; } Record;
extern Record D_801C36EC[30]; /* Complete table; this iterator visits only the first 29. */
extern unsigned char D_801C35E0[0x10C];

void *func_800A910C(Iter *it)
{
    s32 index = it->index;
    if (index == 29) {
        it->index = 30;
        return D_801C35E0;
    } else {
        it->index = index + 1;
        return &D_801C36EC[index];
    }
}
