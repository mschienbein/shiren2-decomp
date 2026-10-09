#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 pad_00[0x1E]; u8 flags_1E; u8 pad_1F[0x53]; u8 flags_72; } Actor;
typedef struct { u8 kind; } Item;
typedef struct { void *source; Pos previous; u8 direction; u8 pad_0D[3]; Pos position; u8 pad_18[4]; u8 field_1C, field_1D; u8 pad_1E[6]; u8 flags_24, field_25; } Plan;
extern u8 D_80147620[];
extern void func_800498E4(s32, ...);
extern u32 func_800B1C6C(Pos *);
extern Actor *func_800B4928(Pos *);
extern Item *func_800B4D80(Pos *);
extern s32 func_800C587C(void *, u8);
extern u16 func_800E08B0(void *);
extern u16 func_800E08F0(void *);
extern s32 func_800EC630(void *, void *);
extern s32 func_8010BEC4(void *, u8);
extern s32 func_8010FC28(void *, void *, void *, void *);
extern s32 func_801368B4(Pos *, s32);
/* ODD_C: signed-byte normalization keeps masked predicates as zero-based
 * nonzero tests instead of shift extraction; both predicates shape scheduling. */
static inline signed char nonzero(u32 value) { return value != 0; }
static inline u8 iszero(u32 value) { return value == 0; }
s32 func_8010F580(void *self, Actor *actor, Plan *plan) {
    Pos *position;
    s32 enabled;
    if (plan->flags_24 & 1) {
        Pos *position = &plan->position;
        if (!(func_800B1C6C(position) & 0x4000)) {
            Actor *target = func_800B4928(position);
            s32 reflecting = 0;
            if (target && ((target->flags_1E >> 4) & 1)) reflecting = nonzero(target->flags_72 & 8);
            enabled = 0;
            if (reflecting) {
                if ((u8)func_8010BEC4(self, 0x47)) enabled = func_800C587C(D_80147620, 0x19) != 0;
                if (enabled) return 2;
            }
        }
    }
    position = &plan->position;
    if (plan->flags_24) return 0;
    if (func_800B1C6C(position) & 0x4000) {
        if (plan->field_1C) {
            s32 straight = 0;
            if ((plan->direction ^ 1) & 1) straight = iszero(func_800B1C6C(&plan->previous) & 0x4000);
            if (straight) {
                enabled = 0;
                if (func_801368B4(position, 0x4000)) enabled = !func_801368B4(position, 0x8020);
                if (enabled) return 1;
                func_800498E4(0x22C);
            }
        }
        return 0;
    }
    {
        s32 collision = 0;
        if ((actor->flags_1E >> 2) & 1) collision = !func_8010FC28(self, actor, plan, &plan->previous);
        if (collision && plan->field_1D) {
            Item *item = func_800B4D80(position);
            if (item && (item->kind == 0x10 || item->kind == 0xA) && func_800EC630(actor, item)) return 3;
        }
    }
    {
        s32 healthy = 0;
        if (!plan->field_25 && func_800E08B0(actor) == func_800E08F0(actor))
            healthy = nonzero((u8)func_8010BEC4(self, 5));
        return healthy * 4;
    }
}
