#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { char pad[0xE4]; u16 flags; char padE6[2]; s32 mask; } Obj;
extern short D_80156A22;
extern short D_80156A2C;
extern char D_80147620[];
extern void func_800EB598(void *obj, s16 amount, s16 alternate_amount);
extern u8 func_800C57A0(void *);
extern void func_80136908(s32 *);
extern void func_800498E4(s32 message_id, ...);
extern void func_800E075C(Obj *);
extern s16 func_800E0D28(void *obj, s16 amount, s16 heal);
/* Actor-effect slot +0x44 supplies self and actor; this override ignores self. */
void func_801202D0(void *unused, Obj *o) {
    u8 roll;
    s32 bits;
    func_800EB598(o, D_80156A22, D_80156A2C);
    roll = func_800C57A0(D_80147620);
    func_80136908(&bits);
    if (roll < 0x24) {
        bits |= 0x104;
        func_800498E4(0x109);
    } else if (roll < 0x48) {
        bits |= 8;
        func_800498E4(0x10A);
    } else if (roll < 0x6C) {
        o->flags |= 4;
        func_800498E4(0x10B);
    } else if (roll < 0x90) {
        o->flags |= 1;
        func_800498E4(0x10C);
    } else if (roll < 0xB5) {
        func_800E075C(o);
        func_800498E4(10);
    } else if (roll < 0xDA) {
        func_800E0D28(o, 1, 1);
    } else {
        o->flags |= 2;
        func_800498E4(0x10D);
    }
    {
        s32 b = bits;
        o->mask |= b;
    }
}
