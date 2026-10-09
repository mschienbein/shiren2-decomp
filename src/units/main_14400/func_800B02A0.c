#include "common.h"

extern void *D_80153AE4[7];
extern void func_800B0054(void *table, void *stream);

void func_800B02A0(void *stream)
{
    s32 index = 1;
    do {
        void *table = D_80153AE4[index++];
        func_800B0054(table, stream);
    } while (index < 7);
}
