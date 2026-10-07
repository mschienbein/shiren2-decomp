#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 pad[0x1E]; u8 unk1E; } S;
s32 func_800A7D14(S *arg0) { return arg0->unk1E & 1; }
