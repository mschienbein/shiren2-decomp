#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    s32 x;
    s32 y;
} Vec2;
typedef struct {
    u8 pad[8];
    s16 delta;
    s16 padA;
    void (*destroy)(void *, s32);
} VTable;
typedef struct {
    u8 pad[8];
    VTable *vtable;
} Obj;
void *func_800B221C(Vec2 *);
s32 func_800AD714(Obj *, Vec2 *);
u32 func_800B1C6C(Vec2 *);
s32 func_800A23E8(Vec2 *, Vec2 *);
s32 func_800AD8AC(Obj *, Vec2 *);
void func_800D3650(Obj *);

s32 func_800ADF20(Obj *obj, Vec2 *avoid)
{
    s32 range = 5;
    Vec2 pos;

    for (;;) {
        s32 i;

        if (range <= 0) {
            break;
        }
        i = 0;
        for (;;) {
            s32 bad;
            s32 near;

            if (i >= 100) {
                break;
            }
            func_800B221C(&pos);
            bad = 0;
            if (!func_800AD714(obj, &pos)) {
                bad = 1;
            } else if (func_800B1C6C(&pos) & 0x2000) {
                bad = 1;
            }
            if (!bad) {
                near = 0;
                if (avoid->y | avoid->x) {
                    Vec2 *p = &pos;
                    Vec2 tmp;

                    tmp.x = p->x;
                    tmp.y = p->y;

                    near = func_800A23E8(avoid, &tmp) < range;
                }
                if (!near) {
                    return func_800AD8AC(obj, &pos);
                }
            }
            i++;
        }
        range -= 4;
    }
    func_800D3650(obj);
    if (obj != 0) {
        obj->vtable->destroy((u8 *)obj + obj->vtable->delta, 3);
    }
    return 0;
}
