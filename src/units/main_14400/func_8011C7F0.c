#include "map_unit_iterator.h"

typedef struct { void *source; u32 kind, auxiliary; unsigned short amount, options; unsigned char scale; } Action;
typedef struct { MapPos position; } Actor;
extern s32 D_80147620[];
extern unsigned short D_801569C4;
extern unsigned short D_801569C6;
extern u32 func_800B1C6C(MapPos *);
extern s32 func_800A6E90(Actor *);
extern s32 func_800A692C(Actor *, s32);
extern s32 func_800A44F4(Actor *, Actor *);
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...);
extern s32 func_800C5954(void *, unsigned short, unsigned short);
extern void func_80136910(Action *, void *, u32, u32, u32);
extern void func_800A7ADC(Actor *, Action *);

/* ODD_C: damage-record initializer helper (as in func_8011ED94's init_damage); passing the
   record through the inline parameter also keeps GCC from hoisting &action into a saved register. */
static inline void init_action(Action *action, Actor *source, s32 amount) {
    func_80136910(action, source, (short)amount, 6, 0x1008);
}

static inline MapPos *copy_pair(MapPos *destination, MapPos *source) {
    destination->x = source->x;
    destination->y = source->y;
    return destination;
}

void func_8011C7F0(void *unused /* unused receiver */, Actor *actor, void *other /* unused scroll callback input */) {
    MapPos origin;
    MapUnitIterator iterator;
    /* Search area; once func_800A9204 has copied it into the iterator, its first corner is
       reused as the scratch position of each visited unit (as in func_80048B04). */
    MapRect area;
    s32 any = 0;
    area = func_800B3080(copy_pair(&origin, &actor->position));
    func_800A9204(&iterator, &area, &actor->position);
    while (func_800A9284(&iterator, 0x7C)) {
        Actor *target = func_800A942C(&iterator);
        s32 eligible = 0;
        area.min.x = target->position.x;
        area.min.y = target->position.y;
        if (!(func_800B1C6C(&area.min) & 0x4000) &&
            !func_800A6E90(target) && !func_800A692C(target, 10)) {
            eligible = func_800A44F4(actor, target) == 2;
        }
        if (eligible) {
            Action action;
            if (!any) {
                func_80049CB4(0x11D, &origin);
                func_80049CB4(0x12D);
            }
            func_80049CB4(0xFF, &area.min);
            init_action(&action, actor, func_800C5954(D_80147620, D_801569C4, D_801569C6));
            any = 1;
            func_80049CB4(6);
            func_800A7ADC(target, &action);
            func_80049CB4(7);
            func_80049CB4(0x129, 15);
        }
    }
    if (!any) {
        func_80049CB4(0x132);
        func_800498E4(0x223);
    }
}
