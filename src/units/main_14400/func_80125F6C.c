#include "common.h"
typedef struct { s32 x, y; } Pair;
typedef struct { Pair corner[2]; } Rectangle;
/* Row-major cursor walked by func_800A3610: current, start and end corners. */
typedef struct { Pair current; Pair start; Pair end; } PairIter;
/* g++ vtable entry; entry 1 (+0x08) is the destructor, void (receiver, s32 flags). */
typedef struct { short delta; short index; void (*destroy)(void *self, s32 flags); } VtEntry;
typedef struct { Pair position; char fields_08[0x1C]; VtEntry *vtable; } Object;
typedef struct { unsigned char kind; char fields_01[7]; VtEntry *vtable; } Item;
typedef struct Owner Owner;
extern Object *func_800AA684(void);
extern s32 func_800A5D2C(void *object, Pair *output, s32 flags);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800A251C(Pair *a, Pair *b);
extern void func_800A5A70(void *object, u32 value);
extern void func_800A58FC(void *actor, Pair *position);
extern void func_800498E4(s32 id, ...);
extern void *func_800B1F90(void *pos);
extern void func_800A30AC(Pair *output, void *input, unsigned char index);
extern Pair *func_800A3610(Pair *out, PairIter *it);
extern void *func_800B4D80(Pair *p);
extern s32 func_800A4314(Object *object, Pair *position);
extern void func_800AD868(Pair *pos);
static inline void copy_pair(Pair *destination, const Pair *source) {
    destination->x = source->x;
    destination->y = source->y;
}
static inline s32 pairs_differ(Pair *a, Pair *b) { return func_800A251C(a, b) ^ 1; }
static inline void destroy(void *receiver, VtEntry *vtable) {
    vtable[1].destroy((char *)receiver + vtable[1].delta, 3);
}
static inline s32 iter_more(PairIter *it) { return it->current.x <= it->end.x; }
/* Item kinds 0x0F, 0x10 and 0x13 are never picked up by this effect. */
static inline s32 usable_item(Item *item) {
    s32 result = 0;
    if (item) {
        unsigned char kind = item->kind;
        if (kind != 0x10) {
            if (kind != 0xF) {
                s32 other = kind != 0x13;
                result = other;
            }
        }
    }
    return result;
}
/* Trap slot +0x44: s32 (self, actor, source position, effect position, direction,
 * target unit, item). This handler reads only the effect position and the item. */
s32 func_80125F6C(Owner *self, void *actor, void *source_position, Pair *target,
                  void *direction, void *target_unit, Item *item) {
    Pair position;
    Rectangle bounds;
    PairIter it;
    s32 changed, message;
    if (item) {
        Object *object = func_800AA684();
        copy_pair(&position, target);
        if (object) {
            if (func_800A5D2C(object, &position, 10)) {
                func_80049CB4(0xCD, item, target);
                func_80049CB4(6);
                func_80049CB4(0x109, target);
                func_80049CB4(7);
                if (pairs_differ(&position, target)) {
                    object->position = *target;
                    func_800A5A70(object, 2);
                    func_80049CB4(6);
                    func_80049CB4(0x88, object);
                    func_80049CB4(7);
                    func_80049CB4(0x8C, object, target, &position);
                }
                func_800A58FC(object, &position);
                func_80049CB4(0x12D);
                destroy(item, item->vtable);
                return 0;
            }
            destroy(object, object->vtable);
        }
        message = 0x223;
    } else {
        Rectangle *area;
        changed = 0;
        area = func_800B1F90(target);
        if (area) bounds = *area;
        else func_800A30AC(bounds.corner, target, 1);
        position.x = bounds.corner[0].x;
        position.y = bounds.corner[0].y;
        it.start = position;
        it.current = it.start;
        position.x = bounds.corner[1].x;
        position.y = bounds.corner[1].y;
        it.end = position;
        while (iter_more(&it)) {
            Item *found;
            func_800A3610(&position, &it);
            found = func_800B4D80(&position);
            if (usable_item(found)) {
                Object *object = func_800AA684();
                if (object) {
                    if (func_800A4314(object, &position)) {
                        func_80049CB4(0x109, &position);
                        func_800A58FC(object, &position);
                        func_800AD868(&position);
                        if (found) destroy(found, found->vtable);
                        changed = 1;
                    } else {
                        destroy(object, object->vtable);
                    }
                }
            }
        }
        message = 0x223;
        if (changed) message = 0xF7;
    }
    func_800498E4(message);
    return 1;
}
