#include "common.h"
typedef signed char s8;
typedef struct { s32 x; s32 y; } Pos800BF738;
typedef struct { Pos800BF738 min; Pos800BF738 max; } Rect800BF738;
typedef struct { short offset; short pad; void (*func)(void *self, s32 mode); } VtEntry800BF738;
typedef struct { unsigned char pad0[8]; VtEntry800BF738 entry_8; } Vtable800BF738;
typedef struct { s32 field_0; s32 field_4; Vtable800BF738 *vtable; } Obj800BF738;
typedef struct { unsigned char pad0[0x400]; s8 field_400; } Self800BF738;
extern Rect800BF738 D_801429C0;
void *func_800AC5B4(s32 size, s32 kind);
Obj800BF738 *func_8010DD70(void *mem, s32 id);
s32 func_800AC670(Obj800BF738 *obj);
void *func_800A33DC(void *out, void *rect);
void *func_800B221C(void *out);
void func_800B1BE0(Pos800BF738 *pos, s32 flags);
s32 func_800AD714(Obj800BF738 *obj, Pos800BF738 *pos);
s32 func_800B5BDC(Pos800BF738 *pos);
u32 func_800B1C6C(void *pos);
s32 func_800BAA98(Self800BF738 *self, Pos800BF738 *pos);
s32 func_800BAAE0(Self800BF738 *self, Pos800BF738 *pos);
void func_800AD7E0(Obj800BF738 *obj, Pos800BF738 *pos, s32 a);
Obj800BF738 *func_800AB0C4(void);

static inline void Pos_copy(Pos800BF738 *dst, Pos800BF738 *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void Rect_copy(Rect800BF738 *dst, Rect800BF738 *src) {
    Pos_copy(&dst->min, &src->min);
    Pos_copy(&dst->max, &src->max);
}

void func_800BF738(Self800BF738 *self) {
    Rect800BF738 rect;
    Pos800BF738 pos;
    Obj800BF738 *obj = func_8010DD70(func_800AC5B4(0x10, 1), 0xCE);
    if (func_800AC670(obj) != 0) return;
    Rect_copy(&rect, &D_801429C0);
    {
        Pos800BF738 tmp; /* output slot for the first-part position producers */
        if (self->field_400 == 1) {
            func_800A33DC(&tmp, &rect);
            pos = tmp;
            func_800B1BE0(&pos, 0x80);
        } else {
            s32 ok;
            do {
                func_800B221C(&tmp);
                pos = tmp;
                ok = 0;
                if (func_800AD714(obj, &pos) && !func_800B5BDC(&pos) && !(func_800B1C6C(&pos) & 0x2000)
                    && !func_800BAA98(self, &pos)) {
                    ok = func_800BAAE0(self, &pos) == 0;
                }
            } while (!ok);
        }
    }
    func_800AD7E0(obj, &pos, 0);
    obj = func_800AB0C4();
    if (obj != 0) {
        Rect800BF738 area;
        Pos800BF738 p;
        s32 tries = 100;
        Rect_copy(&area, &D_801429C0);
        while (1) {
            Pos800BF738 tmp;
            if (--tries == -1) break;
            func_800A33DC(&tmp, &area);
            p = tmp;
            if (func_800B1C6C(&p) & 0x4000) {
                func_800AD7E0(obj, &p, 0);
                break;
            }
        }
        if (tries < 0 && obj != 0) {
            obj->vtable->entry_8.func((char *)obj + obj->vtable->entry_8.offset, 3);
        }
    }
}
