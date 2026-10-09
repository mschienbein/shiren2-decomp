#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { u8 area, floor, part; } Location;
typedef struct { u8 pad0[8]; short adjust_8; short padA; void (*destroy_C)(void *, s32); } VTable;
typedef struct { u8 pad0[0x24]; VTable *field_24; u8 pad28[0x72]; unsigned short flags_9A; } Object;
extern Location D_80148780[3];
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
typedef struct { Position first, last; } Rect;
extern SelectionSave D_80142F24;
extern Rect D_801429C0;
u8 func_800A9958(void);
void func_801224E8(u8 *location);
s32 func_800AA8D0(void);
s32 func_800D5374(s32 index);
s32 func_801225B0(void);
void *func_800AC5B4(s32 size, s32 alternate);
Object *func_80122440(Object *self);
s32 func_800AC670(Object *self);
void func_8012286C(Object *self, u8 value);
s32 func_800AE2A4(Object *self, s32 allowItem, s32 allowWater, void *area);
Object *func_800D5514(s32 index);
s32 func_800A5B98(Object *self, Position *position);
s32 func_800B4F74(Position *position);
s32 func_800A5FCC(Object *self, Position *position);
void func_800A58FC(void *self, Position *position);
static inline s32 sameLocation(Location *location) {
    u8 *part = &location->part;
    s32 valid = 0;
    if (location->area == D_80142F24.index && location->floor == func_800A9958()) valid = *part == D_80142F24.count;
    return valid;
}
static inline s32 inverse(s32 value) { return value ^ 1; }
void func_80122600(void) {
    s32 index;
    Location *location = D_80148780;
    Position position;
    for (index = 0; ; location++, index++) {
        s32 create;
        Object *self;
        if (index >= 3) break;
        if (!sameLocation(location)) continue;
        func_801224E8((u8 *)location);
        create = 0;
        if (func_800AA8D0() && func_800D5374(index)) create = func_801225B0() == 0;
        if (create) {
            Object *created = func_80122440(func_800AC5B4(0x2C, 0));
            if (inverse(func_800AC670(created))) {
                func_8012286C(created, index);
                func_800AE2A4(created, 0, 0, &D_801429C0);
            }
        }
        self = func_800D5514(index);
        if (self) {
            s32 placed = 0;
            s32 retries = 100; /* at most 100 placement attempts */
            self->flags_9A |= 0x140;
            while (retries-- > 0) {
                if (func_800A5B98(self, &position)) {
                    if (inverse(func_800B4F74(&position))) {
                        func_800A58FC(self, &position);
                        placed = 1;
                        break;
                    }
                } else {
                    if (func_800A5FCC(self, &position)) {
                        func_800A58FC(self, &position);
                        placed = 1;
                    }
                    break;
                }
            }
            if (!placed && self) {
                VTable *table = self->field_24;
                table->destroy_C((u8 *)self + table->adjust_8, 3);
            }
            break;
        }
    }
}
