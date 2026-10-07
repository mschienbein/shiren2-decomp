#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 a; s32 b; } Pair;
typedef struct { u8 data[8]; } Ray;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad0[0xA]; u8 field_A; u8 padB[0x67]; u8 field_72; } Obj;
void *func_800A27A4(void *out, void *from, void *to);
void *func_800A2594(void *out, void *from, Dir dir);
s32 func_800B4888(Ray *ray);
s32 func_800A8FC8(s32 *iter, s32 kind);
Obj *func_800A910C(s32 *iter);
s32 func_800A4520(Obj *obj, Obj *self);
s32 func_800E2044(Obj *obj);
s32 func_800A41EC(Obj *obj, Ray *ray);
s32 func_800E20CC(void *obj);
s32 func_800F0EC4(Obj *obj);
void func_800A58FC(Obj *obj, Ray *ray);
void *func_800A6538(void *out_direction, void *obj, void *target);
void func_800A665C(Obj *obj, u8 *dir);
s32 func_80049CB4(s32 id, ...);
Obj *func_801014EC(Obj *self, Pair *target, Pair *origin) {
    Pair from;
    Pair *fp = &from;
    Ray ray;
    Pair to;
    Dir dir;
    s32 iter;
    Obj *obj;
    s32 found;

    from.a = origin->a;
    fp->b = origin->b;
    to.a = target->a;
    to.b = target->b;
    func_800A27A4(&dir, fp, &to);
    func_800A2594(&ray, fp, dir);
    if (func_800B4888(&ray) == 0) {
        iter = 0;
        while (func_800A8FC8(&iter, 0x10) != 0) {
            obj = func_800A910C(&iter);
            found = 0;
            if (obj->field_A == 0x3C && func_800A4520(obj, self) != 0 && (Pair *)obj != origin
                && func_800E2044(obj) != 0 && func_800A41EC(obj, &ray) != 0 && func_800E20CC(obj) == 0) {
                found = func_800F0EC4(obj) == 0;
            }
            if (found) {
                u8 dest;

                func_800A58FC(obj, &ray);
                func_800A6538(&dest, obj, target);
                func_800A665C(obj, &dest);
                func_80049CB4(6);
                func_80049CB4(0x1080, obj);
                func_80049CB4(7);
                obj->field_72 |= 4;
                return obj;
            }
        }
    }
    return 0;
}
