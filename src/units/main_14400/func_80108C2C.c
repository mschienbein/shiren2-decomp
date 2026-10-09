#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { void *source; s32 kind, auxiliary; short amount; u16 flags; u8 scale; } Damage;
typedef struct { s32 kind; u8 pad_04[12]; Damage *payload_10; } Message;
typedef struct { u8 pad_00[0x28]; u16 field_28; } Actor;
extern u16 D_801F5D10, D_8014767C;
extern u8 D_801531A0[];
extern s32 func_800F27A4(void *, Message *);
extern u16 func_800E08B0(void *);
extern s32 func_800E1E20(void *);
extern s32 func_80049CB4(s32, ...);
extern char *func_800A3B20(void *);
extern void func_800498E4(s32, ...);
extern void func_800E1ED0(void *);
extern void func_80049A04(u16, ...);
extern s32 func_801F258C(s32, s32);
extern void func_800F16C0(void *, s32);
extern void func_800E42AC(void *, Damage *);
static inline s32 message_kind(Message *message) { return message->kind; }
s32 func_80108C2C(Actor *self, Message *message) {
    Damage *damage;
    s32 blocked;
    switch (message_kind(message)) {
    case 9:
        func_800F27A4(self, message);
        blocked = 0;
        if (!func_800E08B0(self) || D_801F5D10) blocked = 1;
        if (blocked) return 1;
        if (message->payload_10->source) {
          if (func_800E1E20(self)) {
            func_80049CB4(0x1129, 8);
            func_800498E4(0x156, func_800A3B20(self));
            func_80049CB4(0xAC, self);
            func_800E1ED0(self);
            func_80049CB4(0x1F, self);
            func_80049CB4(0x132);
            func_80049A04(0x157, func_800A3B20(self));
            func_80049CB4(0x129, 0x14);
        }
        }
        return 1;
    case 10:
        if (func_801F258C(0x56, 0)) {
            damage = message->payload_10;
            if (!(D_801531A0[damage->kind] & 0x20)) func_800F16C0(self, damage->kind == 0x27);
            func_800E42AC(self, damage);
            self->field_28 = 1;
            D_8014767C |= 0x40;
            return 1;
        }
        break;
    case 0x15:
        return 0;
    }
    return func_800F27A4(self, message);
}
