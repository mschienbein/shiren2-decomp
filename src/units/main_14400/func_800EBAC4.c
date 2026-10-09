#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { char pad0[0x60]; s16 offset60; s16 pad62; void (*call64)(void *); char pad68[0x28]; s16 offset90; s16 pad92; s32 (*call94)(void *, s32, s32, u8, s32); } Table;
typedef struct { s32 x; s32 y; } Point;
typedef struct Obj { Point point; char pad8[2]; u8 fieldA; char padB[0x19]; Table *field24; char pad28[0x30]; struct Obj *field58; char pad5C[0x18]; u8 field74; char pad75[0x8F]; struct Obj *field104; } Obj;
typedef struct { s32 value; } Direction;
extern s32 func_800E1CD4(Obj *, s32);
extern s32 func_80049CB4(s32, ...);
extern Direction *func_800A65E4(Direction *, Obj *, Obj *);
extern void func_800A665C(Obj *, Direction *);
extern char *func_800A3B20(Obj *);
extern void func_800498E4(s32, ...), func_800FE3FC(Obj *, Obj *), func_800E2298(Obj *), func_800C94E8(void);
extern s32 func_800A8FC8(s32 *, s32), func_800A44F4(Obj *, Obj *);
extern Obj *func_800A910C(s32 *);
static inline void copy_point(Point *out, Point *in) { out->x = in->x; out->y = in->y; }
static inline void update_direction(Obj *p, Obj *other, Direction *direction) { func_800A65E4(direction, p, other); func_800A665C(p, direction); }
static inline void clear_if_owned(Obj *entry, Obj *p) { s32 flag = 0; if (entry->field58 == p->field104) flag = func_800A44F4(entry, p) == 1; if (flag) entry->field58 = 0; }
s32 func_800EBAC4(Obj *p, Obj *other) {
    Point start, end; Direction direction; s32 iterator; s32 blocked = 0;
    if (other == p || func_800E1CD4(other, 15) || p->field104) blocked = 1;
    if (!blocked) { Table *t; char *name;
        func_80049CB4(306); update_direction(other, p, &direction); func_80049CB4(139, p);
        if (func_800E1CD4(other, 12)) { t = other->field24; t->call94((char *)other + t->offset90, 1, 12, 0, 0); }
        p->field104 = other; t = p->field24; t->call64((char *)p + t->offset60); func_80049CB4(31, p);
        copy_point(&start, &p->point); end.x = other->point.x; end.y = other->point.y;
        func_80049CB4(27, p, &start, &end); func_80049CB4(147, other); func_80049CB4(221);
        name = func_800A3B20(p); func_800498E4(232, name, func_800A3B20(other));
        if (other->fieldA == 46) func_800FE3FC(other, p);
        func_80049CB4(31, p->field104); func_80049CB4(137, p); func_800E2298(other); func_800C94E8();
        ++p->field74; iterator = 0;
        for (;;) { s32 ready = func_800A8FC8(&iterator, 124); if (!ready) break; clear_if_owned(func_800A910C(&iterator), p); }
        return 1;
    }
    return 0;
}
