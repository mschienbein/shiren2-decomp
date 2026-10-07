#include "common.h"

typedef struct Actor Actor;

/* Partial class D_8015A7C0 view: +0xA0 stores an actor pointer. */
typedef struct {
    unsigned char pad0[0xA0];
    Actor *field_A0;
} Obj800FD914;

Actor *func_800FD914(Obj800FD914 *obj) {
    return obj->field_A0;
}
