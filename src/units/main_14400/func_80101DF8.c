#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { Position position; } Actor;
typedef struct { u8 pad0[2]; u8 flags; } Item;

u8 D_80148430[16] = { 0xDF, 0xDD, 0xD4, 0xDE };
s32 func_800B4F74(Position *position);
s32 func_800E20CC(void *self);
s32 func_800E0F40(Actor *self);
void *func_800AC244(u8 id);
void func_800AD7E0(Item *self, void *position, s32 notify);
s32 func_800A60D8(void *self, Position *position);
s32 func_80049CB4(s32 id, ...);
void func_800A59A4(Actor *self);
void func_800A58FC(void *self, Position *position);
static inline Position *capture(Position *out, Actor *self) {
    out->x = self->position.x; out->y = self->position.y; return out;
}
/* Monster action slot +0xB4 supplies a target pointer; this override ignores it. */
s32 func_80101DF8(Actor *self, void *target) {
    Position position;
    Position *p = capture(&position, self);
    s32 excluded;
    Item *item;
    if (func_800B4F74(p)) return 0;
    excluded = 0;
    if (!func_800E20CC(self)) {
        s32 ordinary = D_80142F18.mode == 0x4F;
        excluded = ordinary ^ 1;
    }
    if (excluded) return 0;
    if ((D_80142F18.mode ^ 0x4F) == 0) {
        func_80049CB4(0x105B, self);
        return 1;
    }
    item = func_800AC244(D_80148430[(u8)func_800E0F40(self) - 1]);
    if (item) { func_800AD7E0(item, p, 1); item->flags &= 0xEF; }
    if (func_800A60D8(self, p)) {
        func_80049CB4(0x105B, self);
        func_80049CB4(0x132);
        func_80049CB4(0x87, self);
        func_800A59A4(self);
        self->position = position;
        func_80049CB4(0x93, self);
        func_80049CB4(0xDD);
        func_80049CB4(0x82, self, p);
        func_800A58FC(self, p);
    }
    return 1;
}
