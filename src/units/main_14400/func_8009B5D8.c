#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pos;
typedef struct VTable { u8 pad_00[0x80]; short adjustment_80; short pad_82; void (*move_84)(void *, Pos *); } VTable;
typedef struct Obj8009B338 { u8 pad_00[0x34]; Pos pos_34; u8 pad_3C[0x10]; const VTable *vtable_4C; u8 pad_50[0x30]; s32 mode_80; u8 pad_84[4]; s32 enabled_88; } Obj8009B338;
extern const u8 D_80152894[12], D_801528A0[12];
extern const s32 D_801528AC[4];
extern void func_80045A24(s32 sound);
void func_8009B5D8(Obj8009B338 *self, s32 direction) {
    Pos pos = self->pos_34;
    s32 limit = self->mode_80 == 2 ? 4 : 6;
    s32 next, previous;
    switch (direction) {
    case 0:
        if (pos.x < limit) ++pos.x; else pos.x = 0;
        break;
    case 1:
        if (pos.x > 0) --pos.x; else pos.x = limit;
        break;
    case 2:
        if (pos.x) {
            if (pos.y < 9) ++pos.y; else pos.y = 0;
        } else {
            next = D_80152894[self->pos_34.y];
            ++next;
            if (next >= 4) next = 0;
            if (!self->enabled_88 && next == 1) next = 2;
            next = D_801528AC[next];
            pos.y = next;
        }
        break;
    case 3:
        if (pos.x) {
            if (pos.y > 0) --pos.y; else pos.y = 9;
        } else {
            if (self->enabled_88) previous = D_80152894[self->pos_34.y] - 1;
            else previous = D_801528A0[self->pos_34.y] - 1;
            if (previous < 0) previous = 3;
            pos.y = D_801528AC[previous];
        }
        break;
    default:
        return;
    }
    self->vtable_4C->move_84((u8 *)self + self->vtable_4C->adjustment_80, &pos);
    func_80045A24(2);
}
