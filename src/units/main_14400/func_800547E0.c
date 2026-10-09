#include "common.h"

typedef unsigned char u8;

extern u8 D_80161B44;
typedef struct { s32 field_0, field_4, field_8, field_C, field_10, field_14, field_18, field_1C; char messages_20[4][0x100]; } Slot;
extern Slot D_80161724[1];
void func_80053D70(s32, s32, s32, s32, s32, s32, char *, void *);
void func_800547E0(s32 mode, char *fmt, void *args) {
    D_80161B44 = 1;
    func_80053D70(0, mode, 5, 0x15, 0x1E, 6, fmt, args);
    D_80161724[0].field_8 = 2;
}
