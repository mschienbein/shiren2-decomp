#include "common.h"

typedef unsigned char u8;
typedef struct { u8 storage[0xA8]; } ObjectTable;
extern ObjectTable D_80142DE0;
extern void *func_800AB16C(void *table, u8 kind, s32 arg);

void *func_800AAD88(u8 kind, s32 arg)
{
    return func_800AB16C(&D_80142DE0, kind, arg);
}
