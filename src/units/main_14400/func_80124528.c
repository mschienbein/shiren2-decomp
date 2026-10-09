#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { u8 pad_00[8]; short delta_08, reserved_0A; void (*destroy_0C)(void *, s32); } ItemVTable;
typedef struct { u8 pad_00[8]; ItemVTable *vtable_08; } Item;
typedef struct { u8 pad_00[0x10]; u8 activated_10; } Trap;
typedef struct { u8 pad_00[0x1E]; u8 type_1E; u8 pad_1F[0xC5]; u16 flags_E4; } Entity;
typedef struct { void *source_00; u32 kind_04, flags_08; u16 amount_0C, extra_0E; u8 mode_10; } Damage;
extern u8 D_80156A5D, D_80156A5F;
extern u8 D_80147620[];
extern u16 D_8014767C;
extern s32 D_80147678;
void *func_800B4D80(Position *position);
s32 func_80049CB4(s32 id, ...);
void func_800D3650(void *item);
s32 func_800C5844(void *rng, u8 low, u8 high);
s32 func_800E1CD4(void *target, s32 kind);
u16 func_80115944(void *owner, u16 value);
void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
void func_800EDE50(u8 *target, s32 silent);
u16 func_800E08B0(void *target);
s32 func_800AA03C(void);
s32 func_800E1DA0(void *target);
void func_800EBCD0(void *target);
s32 func_800A7024(u8 *target);
void func_800498E4(s32 id, ...);
void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 extra);
void func_800A7B68(Entity *target, Damage *damage);
/* The seven-pointer effect slot supplies origin and direction even though unused here. */
s32 func_80124528(Trap *trap, void *source, Position *origin, Position *position, u8 *direction, Entity *target, Item *item)
{
    s32 result = 1;
    if (func_800B4D80(position) == trap) {
        trap->activated_10 = 1;
        func_80049CB4(0x10E2, position);
    }
    if (item) {
        func_80049CB4(0xCD, item, position);
        func_80049CB4(0x1131);
        func_80049CB4(6);
        func_80049CB4(0xC4, item, position);
        func_80049CB4(7);
        func_800D3650(item);
        item->vtable_08->destroy_0C((u8 *)item + item->vtable_08->delta_08, 3);
        result = 0;
    }
    if (target) {
        if (target->type_1E & 0xC) {
            s32 amount, eligible;
            func_80049CB4(0x1073, target);
            if ((target->type_1E >> 2) & 1) {
                target->flags_E4 |= 0x100;
            }
            amount = (u8)func_800C5844(D_80147620, D_80156A5D, D_80156A5F);
            eligible = func_800E1CD4(target, 15) != 1;
            if (eligible) {
                func_800A7B18(target, source, (u16)func_80115944(trap, amount), 10);
            }
            if ((target->type_1E >> 2) & 1) {
                Entity *player = target;
                s32 survives;
                if ((target->flags_E4 >> 6) & 1) {
                    func_800EDE50((u8 *)target, 1);
                    D_8014767C |= 0x40;
                }
                survives = 0;
                if (func_800E08B0(target)) {
                    survives = func_800AA03C() != 0;
                }
                if (survives) {
                    if (func_800E1DA0(target)) {
                        func_800EBCD0(target);
                    } else {
                        D_80147678 = 7;
                    }
                    player->flags_E4 &= 0xFEFF;
                    return result;
                }
                target->flags_E4 &= 0xFEFF;
            }
            if (func_800E08B0(target)) {
                func_80049CB4(0x1074, target);
                func_80049CB4(0x1F, target);
            }
        } else {
            Damage damage;
            Damage *payload;
            if (func_800A7024((u8 *)target)) {
                func_800498E4(0x224);
                return result;
            }
            func_80049CB4(0x1073, target);
            payload = &damage;
            func_80136910(payload, source, 1, 10, 0);
            func_80049CB4(0x87, target);
            func_800A7B68(target, payload);
        }
    }
    return result;
}
