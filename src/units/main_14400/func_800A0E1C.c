#include "common.h"

typedef s32 (*Predicate)(void *);

/* Selection request prefix: the first word is the owning list object. */
typedef struct {
    void *owner;
} Object;

/* Selection dialog object at 0x801404E0 (same Context view as func_800A18AC).
 * Its constructor at 0x80097E7C clears the predicate at +0x2EC and the mode
 * at +0x408; the mode is read back with lhu (0x80098B48). */
typedef struct {
    char pad0[0x2CC];
    s32 field2CC;
    char pad2D0[0x1C];
    Predicate callback;
    char pad2F0[0x118];
    unsigned short field408;
} Context;

extern s32 func_800A0E00(void **owner_slot, Predicate callback);
extern void func_80097B90(void *object, void *owner, void *holder, s32 mode, void *description, s32 flag);
extern s32 func_800957C0(void *object, void *output, s32 modal, void *history, s32 event);
extern s32 func_8009A0EC(void *object);
extern Context D_801404E0;
/* Two-word menu descriptor {7, 6} (.data 0x80139020..0x80139027), the same shape as
 * D_80139054/D_8013906C: func_80097B90 forwards it to func_80097BE0, which reads +0 and +4
 * (0x80097E98/0x80097E9C). */
extern s32 D_80139020[2];

/* Whole-object setters, integrated at their call sites (absolute stores
 * 0x800A0E98/0x800A0EA0). */
static inline void setContextMode(Context *context, unsigned short mode) {
    context->field408 = mode;
}

static inline void setContextCallback(Context *context, Predicate callback) {
    context->callback = callback;
}

/* mode is the u16 dialog mode stored at Context+0x408. Every original caller
 * passes an in-range constant (0x1E8/0x1E9/0x1EA/0x1EC), and the incoming a2
 * is kept in the promoted-parameter temporary that only a narrow formal
 * produces. */
s32 func_800A0E1C(Object *object, void *output, unsigned short mode, Predicate callback) {
    switch (func_800A0E00(&object->owner, callback) ^ 1) {
    default:
        return -3;
    case 0:
        break;
    }
    func_80097B90(&D_801404E0, object->owner, 0, 0x1000, D_80139020, 0);
    setContextMode(&D_801404E0, mode);
    setContextCallback(&D_801404E0, callback);
    switch (func_800957C0(&D_801404E0, output, 1, 0, 0) ^ 1) {
    default:
        return -1;
    case 0:
        break;
    }
    if (func_8009A0EC(&D_801404E0) & 1) {
        return -5;
    }
    return 1;
}
