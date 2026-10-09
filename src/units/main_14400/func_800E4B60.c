#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Event message delivered through the +0x5C dispatch slot of the unit vtable
 * D_80158C98 (entry at 0x80158CF4); payload layout depends on kind. */
typedef struct {
    s32 kind;
    u8 pad4[4];
    void *target_8;
    u8 padC[4];
    union {
        struct {
            void *first;
            void *second;
        } pair;
        struct {
            u8 pad10[2];
            u16 flags;
        } notice;
    } payload_10;
} Event;

s32 func_800E8694(void *unit);
s32 func_800E8350(void *unit);
s32 func_800E66EC(void *unit);
s32 func_800E4F50(void *unit);
void func_800E5020(void *unit);
void func_800E51E0(void *unit, Event *event);
void func_800E5248(void *unit);
s32 func_800E53A8(void *unit);
void func_800E552C(void *unit, void *damage, void *outcome);
s32 func_800E3D20(void *unit, void *attack);
void func_800E42AC(void *unit, void *context);
void func_800E5968(void *unit, void *target, u16 flags);
s32 func_800E5BE0(void *unit, Event *event);
s32 func_800E5A78(void *unit, Event *event);
s32 func_800E5DFC(void *unit, Event *event);
s32 func_800E5E4C(void *unit, s32 notify);
void func_800E5F1C(void *unit, s32 notify);

s32 func_800E4B60(void *self, Event *event) {
    void *attack;

    switch (event->kind) {
    case 0:
        if (func_800E8694(self)) {
            return func_800E8350(self);
        }
        return func_800E66EC(self);
    case 1:
        return func_800E4F50(self);
    case 2:
        func_800E5020(self);
        return 1;
    case 3:
        func_800E51E0(self, event);
        return 1;
    case 4:
        func_800E5248(self);
        return 1;
    case 7:
        return func_800E53A8(self);
    case 8:
        func_800E552C(self, event->payload_10.pair.first, event->payload_10.pair.second);
        return 1;
    case 10:
        attack = event->payload_10.pair.first;
        func_800E3D20(self, attack);
        func_800E42AC(self, attack);
        return 1;
    case 14:
        func_800E5968(self, event->target_8, event->payload_10.notice.flags);
        return 1;
    case 11:
        func_800E5BE0(self, event);
        return 1;
    case 20:
        return func_800E5A78(self, event);
    case 21:
        return func_800E5DFC(self, event);
    case 23:
        return func_800E5E4C(self, 1);
    case 24:
        func_800E5F1C(self, 1);
        return 1;
    }
    return 0;
}
