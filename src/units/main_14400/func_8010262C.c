#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct VtEntry {
    s16 delta;
    s16 index;
    void *pfn;
} VtEntry;

typedef struct Position {
    s32 x;
    s32 y;
} Position;

typedef struct Object {
    Position position;
    u8 pad_08[0x58 - 0x8];
    struct Object *target_58;
    u8 pad_5C[0x89 - 0x5C];
    u8 range_89;
} Object;

/* Attack event built on the stack: func_800C4BC0 base, vtable D_8015B808 (cf. func_80102788). */
typedef struct Event8010262C {
    u8 pad_00[0x8];
    s32 field_08;
    const VtEntry *vtable_0C;
    u8 pad_10[0x8];
} Event8010262C;


extern const VtEntry D_8015B808[];
extern s32 func_800F17A8(Object *, Position *, s32);
s32 func_800A4520(Object *ctx, Object *obj);
s32 func_800E20CC(Object *arg0);
Position *func_800F13A0(Position *ret, Object *unit, s32 range);
s32 func_800F1244(Object *arg, Object *target, s32 distance, s32 force);
u8 func_800A6420(Object *obj, Object *target);
extern s32 func_800A692C(Object *, s32);
void *func_800B4D80(Position *p);
void func_800AD868(Position *pos);
extern s32 func_800A6EE0(void *);
Event8010262C *func_800C4BC0(Event8010262C *s, Object *a, Object *b, u16 c);
extern void func_800C4864(Event8010262C *, s32, void *);

static inline void copyPosition(Position *dest, Position *src) {
    dest->x = src->x;
    dest->y = src->y;
}

static inline Event8010262C *initEvent(Event8010262C *s, Object *a, Object *b, u16 c) {
    func_800C4BC0(s, a, b, c);
    s->vtable_0C = D_8015B808;
    return s;
}

/* Life vtable D_8015B838 slot 0xB0. */
s32 func_8010262C(Object *self, Object *other) {
    Position pos;
    Position dest;
    Event8010262C event;
    Object *found;
    s32 kind;
    s32 failed;
    s32 quiet;

    copyPosition(&pos, &self->position);
    failed = func_800F17A8(self, &pos, 0) != 1;
    if (failed) {
        if (func_800A4520(self, other) != 0) {
            return 0;
        }
        return func_800E20CC(self) ^ 1;
    }
    func_800F13A0(&dest, self, self->range_89);
    switch (func_800F1244(self, self->target_58, self->range_89, 0)) {
    case 1:
        return 0;
    case 2: {
        s32 ok = 0;

        if (func_800A6420(self, self->target_58) != 3) {
            ok = func_800A692C(self, 0x12) == 0;
        }
        if (!ok) {
            return 1;
        }
        break;
    }
    }
    found = func_800B4D80(&pos);
    func_800AD868(&pos);
    quiet = D_80142F18.mode == 0x4F;
    if (quiet) {
        kind = 2;
    } else {
        kind = func_800A6EE0(self);
    }
    initEvent(&event, self, found, kind);
    event.field_08 = 0;
    func_800C4864(&event, 0x29, &dest);
    return 1;
}
