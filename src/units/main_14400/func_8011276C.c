#include "common.h"
typedef struct { s32 type; } Ev;
void func_801127F8(void *a, Ev *e);
void func_80112864(void *a, Ev *e);
void func_801124B4(void *a);
s32 func_8010C96C(void *a, Ev *e);
static inline s32 event_type(Ev *e) { return e->type; }

s32 func_8011276C(void *a, Ev *e) {
    switch (event_type(e)) {
    case 0x12:
        func_801127F8(a, e);
        return 1;
    case 0x13:
        func_80112864(a, e);
        return 1;
    case 0x1A:
        func_801124B4(a);
        return 1;
    default:
        return func_8010C96C(a, e);
    }
}
