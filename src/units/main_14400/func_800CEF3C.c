#include "common.h"
#include "item_records.h"

typedef struct {
    char reserved_00[0x1E];
    unsigned char field_1E;
} Actor;
typedef struct {
    char reserved_00[0x10];
    Actor *field_10;
} State;
extern s32 func_800CD090(State *, Item *);
extern void *func_800E8A68(Actor *, unsigned char);
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);

s32 func_800CEF3C(State *state, Item *item, s32 notify) {
    char *name = func_800AE674(item);
    Actor *actor;
    if (func_800CD090(state, item) < 0) {
        return 0;
    }
    actor = state->field_10;
    if ((actor->field_1E & 0xC) && func_800E8A68(actor, 9)) {
        if (notify) {
            func_800498E4(0xB6, name);
        }
        return 0;
    }
    if (item->flags02 & 4) {
        ItemMethods *methods;
        if (item->field01 == 0xAA) {
            if (notify) {
                func_800498E4(0xB5, name);
            }
            return 0;
        }
        methods = item->field08;
        if (methods->test14((char *)item + methods->delta10)) {
            if (notify) {
                func_80049CB4(0x12B);
                func_800498E4(0x2F, name);
            }
            return 0;
        }
    }
    return 1;
}
