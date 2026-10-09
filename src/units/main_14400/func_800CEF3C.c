#include "common.h"

typedef struct {
    char reserved_00[0x1E];
    unsigned char field_1E;
} Actor;
typedef struct {
    char reserved_00[0x10];
    Actor *field_10;
} State;
typedef struct {
    char reserved_00[0x10];
    short adjustment_10;
    short reserved_12;
    s32 (*method_14)(void *);
} Methods;
typedef struct {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    char reserved_03[5];
    Methods *methods_08;
} Item;
extern char *func_800AE674(Item *);
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
    if (item->field_02 & 4) {
        Methods *methods;
        if (item->field_01 == 0xAA) {
            if (notify) {
                func_800498E4(0xB5, name);
            }
            return 0;
        }
        methods = item->methods_08;
        if (methods->method_14((char *)item + methods->adjustment_10)) {
            if (notify) {
                func_80049CB4(0x12B);
                func_800498E4(0x2F, name);
            }
            return 0;
        }
    }
    return 1;
}
