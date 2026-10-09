#include "common.h"
typedef struct Pos Pos;
typedef struct { unsigned char bytes[0x14]; } Record;
extern Record D_801431F0[];
extern s32 func_800B200C(Pos *pos);
void *func_800B1F90(void *pos) {
    s32 index = func_800B200C(pos);
    return index >= 0 ? &D_801431F0[index] : 0;
}
