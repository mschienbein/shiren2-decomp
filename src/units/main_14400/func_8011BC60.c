#include "common.h"
typedef unsigned short u16;
typedef signed char s8;
typedef struct { s32 x; s32 y; } Pos8011BC60;
typedef struct { Pos8011BC60 min; Pos8011BC60 max; } Rect8011BC60;
typedef struct { Pos8011BC60 start; Pos8011BC60 cur; Pos8011BC60 end; } Iter8011BC60;
typedef struct { unsigned char pad0[5]; s8 field_5; } Unit8011BC60;
s32 func_800B5BDC(Pos8011BC60 *pos);
Rect8011BC60 *func_800B1F90(Pos8011BC60 *pos);
s32 func_80049CB4(s32 id, ...);
void *func_800A3610(Pos8011BC60 *out, Iter8011BC60 *iter);
Unit8011BC60 *func_800B4D80(Pos8011BC60 *pos);
void func_800B4E7C(Pos8011BC60 *pos);
void *func_800B5CC0(Rect8011BC60 *room);
s32 func_800D248C(void *spawner);
void func_800498E4(s32 message_id, ...);

static inline void Pos_copy(Pos8011BC60 *dst, Pos8011BC60 *src) {
    dst->x = src->x;
    dst->y = src->y;
}

/* Item-effect slot +0x44 also supplies an item; this override does not use it. */
void func_8011BC60(Unit8011BC60 *self, Pos8011BC60 *at, void *item) {
    Pos8011BC60 pos;
    u16 msg = 0x223;
    Pos_copy(&pos, at);
    if (func_800B5BDC(&pos) != 0) {
        Rect8011BC60 *room = func_800B1F90(&pos);
        Rect8011BC60 r;
        Iter8011BC60 it;
        Pos8011BC60 p;
        Pos_copy(&r.min, &room->min);
        Pos_copy(&r.max, &room->max);
        Pos_copy(&p, &r.min);
        it.cur = p;
        it.start = it.cur;
        Pos_copy(&p, &r.max);
        it.end = p;
        func_80049CB4(0x129, 0x1F);
        while (1) {
            Unit8011BC60 *u;
            s32 valid = it.start.x <= it.end.x;
            if (!valid) break;
            func_800A3610(&p, &it);
            u = func_800B4D80(&p);
            if (u != 0 && u != self && ~u->field_5 != 0) {
                func_80049CB4(0x109, &p);
                func_800B4E7C(&p);
                func_80049CB4(0xD7, &p);
            }
        }
        if (func_800D248C(func_800B5CC0(room)) != 0) msg = 0xD9;
    }
    if (msg == 0x223) func_80049CB4(0x132);
    func_800498E4(msg);
}
