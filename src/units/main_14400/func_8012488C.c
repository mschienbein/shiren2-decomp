#include "common.h"
typedef struct { s32 x; s32 y; } Pair;
extern s32 func_80049CB4(s32 message, ...);
extern s32 func_800ADF20(void *obj, Pair *avoid);
extern s32 func_800A529C(void *obj, s32 flag);
/* Relocate obj away from a private copy of the position (func_800ADF20 may update it). */
static inline void avoid_from(void *obj, Pair *src)
{
    Pair copy;

    copy.x = src->x;
    copy.y = src->y;
    func_800ADF20(obj, &copy);
}
/* D_8015FED0+0x44 (0x8015FF14): func_80115EB0 supplies seven pointers and consumes
 * the s32 result; pair is the origin, owner arrives in the target slot and obj in
 * the item slot. self, actor, source_position and direction are caller-supplied
 * but unused here. */
s32 func_8012488C(void *self, void *actor, void *source_position, Pair *pair,
                  void *direction, void *owner, void *obj)
{
    Pair first;
    func_80049CB4(0xEB, pair);
    if (obj) {
        func_80049CB4(6);
        func_80049CB4(0xCD, obj, pair);
        func_80049CB4(0xC8, obj, pair);
        first.x = pair->x;
        first.y = pair->y;
        avoid_from(obj, &first);
        func_80049CB4(7);
    }
    if (owner) func_800A529C(owner, 0);
    func_80049CB4(0x132);
    return obj == 0;
}
