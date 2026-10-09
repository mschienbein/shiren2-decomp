#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef short s16;
/* +8 table: destructor +0xC (delta +0x8), message handler +0x3C (delta +0x38). */
typedef struct {
    s32 unk0, unk4;
    s16 unk8, unkA;
    void (*unkC)(void *self, s32 flags);
    char unk10[0x28];
    s16 unk38, unk3A;
    s32 (*unk3C)(void *receiver, void *event);
} Dispatch;
typedef struct { s32 unk0, unk4; Dispatch *unk8; } State;
typedef struct { s32 unk0; void *unk4; char unk8[0x18]; } Message;

u8 D_80148420[16] = { 0x9E, 0x96, 0x9D };
s32 func_800E20CC(void *self);
s32 func_800A44F4(void *self, void *target);
s32 func_800A4520(void *ctx, void *obj);
void *func_800A6BA4(void *obj, s32 range, s32 ignore_terrain);
s32 func_800E0F40(void *self);
void *func_800AC244(u8 id);

static inline Message *make_message(Message *message, void *source) {
    message->unk0 = 5;
    message->unk4 = source;
    return message;
}

s32 func_801012F4(void *self, void *target) {
    Message message;
    s32 eligible = 0;
    State *state;
    if (!func_800E20CC(self)) {
        s32 ordinary = D_80142F18.mode == 0x4F;
        eligible = ordinary ^ 1;
    }
    if (eligible && func_800A44F4(self, func_800A6BA4(self, 0x4C, 0)) != 1) {
        if (target && func_800A4520(self, target)) return 0;
        return 1;
    }
    state = func_800AC244(D_80148420[(u8)func_800E0F40(self) - 1]);
    if (state) {
        Dispatch *dispatch;
        Message *current = make_message(&message, self);
        dispatch = state->unk8;
        dispatch->unk3C((char *)state + dispatch->unk38, current);
        dispatch = state->unk8;
        dispatch->unkC((char *)state + dispatch->unk8, 3);
    }
    return 1;
}
