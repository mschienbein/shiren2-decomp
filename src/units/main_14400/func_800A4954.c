#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef struct Object Object;
typedef struct { u8 pad_0[0x40]; s16 offset; u16 pad_42; s32 (*classify)(void *, Object *, u8 *); } Table;
struct Object { u8 pad_0[0x1E]; u8 flags; u8 pad_1F[5]; Table *table; };
typedef struct { s32 state; Object *source; s32 position[2]; s32 bounds[4]; } Iterator;
typedef struct { u8 value; } Dir;
typedef struct { u8 priority; Dir direction; } Assessment;
extern Iterator *func_800A915C(Iterator *out, Object *source);
extern s32 func_800A9284(Iterator *, s32);
extern Object *func_800A942C(Iterator *);
extern s32 func_800A674C(Object *self, Object *other);
extern s32 func_800E1CC4(Object *obj, s32 kind);
extern s32 func_800A6E90(void *obj);
extern u32 func_800B1C6C(void *obj);
extern s32 func_800A65B8(Object *self, void *other);
extern void *func_800A65E4(Dir *out, Object *obj, void *target);
void *func_800A4954(Object *object, s32 category, u8 *out_priority, s32 use_priority, s32 check_target)
{
    Iterator iterator;
    Assessment assessment;
    Iterator *current;
    Dir *dir;
    Object *best = 0;
    s32 best_distance = 76;
    u8 best_priority = 0;
    assessment.priority = 0;
    func_800A915C(&iterator, object);
    current = &iterator;
    dir = &assessment.direction;
    while (func_800A9284(current, 255) != 0) {
        Object *candidate = func_800A942C(current);
        s32 eligible = 0;
        if (object->table->classify((u8 *)object + object->table->offset, candidate, &assessment.priority) == category &&
            (check_target == 0 || func_800A674C(object, candidate) != 0) &&
            (!(candidate->flags & 124) || func_800E1CC4(candidate, 1) == 0) &&
            func_800A6E90(candidate) == 0 && !(func_800B1C6C(candidate) & 0x4000)) {
            eligible = 1;
        }
        if (eligible != 0) {
            s32 distance;
            if (use_priority == 0) {
                assessment.priority = 0;
            }
            distance = func_800A65B8(object, candidate);
            if (distance == 1) {
                func_800A65E4(dir, object, candidate);
                if ((assessment.direction.value ^ 1) & 1) {
                    distance = 0;
                }
            }
            if (assessment.priority > best_priority || (assessment.priority == best_priority && distance < best_distance)) {
                best = candidate;
                best_distance = distance;
                best_priority = assessment.priority;
            }
        }
    }
    *out_priority = best_priority;
    return best;
}
