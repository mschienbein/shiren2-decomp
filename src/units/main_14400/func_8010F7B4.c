#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0xC]; u8 field_0C; u8 pad_0D[3]; u8 member_10[0xC]; u8 kind_1C; } Event;
extern s32 func_800B43BC(void *pos, s32 arg, u8 kind);
extern void *func_800C5F60(void);
extern void func_800B1CEC(s32 mode, void *pos);
extern s32 func_8010EDD4(void *obj, void *other);
void func_8010F7B4(void *obj, void *other, Event *event) {
    void *member = event->member_10;
    func_800B43BC(member, 1, event->field_0C);
    if (other == func_800C5F60()) func_800B1CEC(1, member);
    if (event->kind_1C == 0x37) func_8010EDD4(obj, other);
}
