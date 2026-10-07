#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[2]; u8 field_2; u8 field_3; u8 field_4; u8 field_5; } Info;
typedef struct { u8 pad0[0x10]; Info *info; } Obj;
extern Obj *D_8013B980;
void func_80066950(s32 *a, s32 *b, s32 *c, s32 *d) {
    if (D_8013B980 != 0) {
        if (a != 0) {
            *a = D_8013B980->info->field_2;
        }
        if (b != 0) {
            *b = D_8013B980->info->field_3;
        }
        if (c != 0) {
            *c = D_8013B980->info->field_4;
        }
        if (d != 0) {
            *d = D_8013B980->info->field_5;
        }
    } else {
        if (a != 0) {
            *a = 0;
        }
        if (b != 0) {
            *b = 0;
        }
        if (c != 0) {
            *c = 0x37;
        }
        if (d != 0) {
            *d = 0x21;
        }
    }
}
