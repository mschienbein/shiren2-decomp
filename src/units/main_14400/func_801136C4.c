#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 field_00, kind_01; u8 pad_02[10]; u8 turns_0C; } Timer;
extern u8 D_80156931, D_80156933;
extern void func_801137CC(Timer *, void *, u8);
extern void func_801138DC(Timer *, void *);
extern void func_8011391C(Timer *, void *);
extern void func_80113A30(Timer *, void *);
void func_801136C4(Timer *timer, void *owner, short damage, s32 kind, u16 flags) {
    if (damage > 0) {
        u8 type = timer->kind_01;
        if (type == 0x8C && (flags & 0x800)) {
            func_80113A30(timer, owner);
            return;
        }
        if (type == 0x8B) {
            switch (kind) {
            case 24: case 25: case 28: case 35: case 36:
                break;
            default:
                func_8011391C(timer, owner);
                return;
            }
        }
        if (timer->turns_0C == 0) {
            switch (kind) {
            case 1:
                func_801137CC(timer, owner, D_80156933);
                break;
            default:
                if (!(flags & 0x400)) break;
            case 8: case 9: case 12: case 13: case 16: case 39:
                func_801137CC(timer, owner, D_80156931);
                break;
            }
        } else if (kind == 1) {
            func_801138DC(timer, owner);
        }
    }
}
