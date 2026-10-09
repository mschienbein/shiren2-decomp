#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { u8 kind_00; u8 pad_01[11]; u8 flags_0C; } Item;
typedef struct { u8 pad_00[0x10]; Position position_10; u8 pad_18[5]; u8 kind_1D; } Action;
void *func_800B4D80(Position *position);
void *func_800B31E8(void *position, s32 team);
s32 func_80049CB4(s32 id, ...);
char *func_800AC990(void *item);
void func_800498E4(s32 id, ...);
void func_800D3650(void *item);
void func_800B4E7C(Position *position);
s32 func_8010EDD4(void *object, void *other);
void func_8010F8A0(void *object, void *other, Action *action)
{
    Position *position = &action->position_10;
    Item *item = func_800B4D80(position);
    if (item && item->kind_00 == 0x10) {
        s32 blocked = 0;
        if (((item->flags_0C >> 1) & 1) || func_800B31E8(position, 0x15)) {
            blocked = 1;
        }
        if (blocked) {
            func_80049CB4(0x109, &action->position_10);
            func_800498E4(0xF3, func_800AC990(item));
            return;
        }
    }
    func_80049CB4(0x131);
    func_80049CB4(6);
    func_80049CB4(0x107, &action->position_10);
    func_80049CB4(7);
    func_80049CB4(0x109, &action->position_10);
    func_80049CB4(0xD7, &action->position_10);
    func_800498E4(0xF2, func_800AC990(func_800B4D80(&action->position_10)));
    func_800D3650(func_800B4D80(&action->position_10));
    func_800B4E7C(&action->position_10);
    if (action->kind_1D == 0x49) {
        func_8010EDD4(object, other);
    }
}
