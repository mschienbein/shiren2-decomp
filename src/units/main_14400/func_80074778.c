#include "common.h"
typedef signed short s16;
typedef unsigned char u8;
typedef struct { s16 unk0; s16 unk2; u8 pad4[0xAC]; } Shadow;
void func_80074778(Shadow *shadow) { shadow->unk2 = -1; }
