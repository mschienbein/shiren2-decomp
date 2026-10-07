#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s32 type_0;
    void *arg_4;
    s32 pad8[2];
    s32 field_10;
    s32 pad14;
} Msg8011F9B0;

typedef struct {
    u8 pad0[0x58];
    s16 delta_58;
    s16 pad5A;
    s32 (*func_5C)(void *self, Msg8011F9B0 *msg);
} VTable8011F9B0;

typedef struct {
    u8 pad0[0x24];
    VTable8011F9B0 *vtable_24;
} Obj8011F9B0;

/* Trap apply slot +0x54 supplies five pointers; self, direction and attacker
 * are unused by this override. The actor remains a pointer in the message. */
void func_8011F9B0(void *self, void *arg, void *target_arg, void *direction, void *attacker) {
    Obj8011F9B0 *target = target_arg;
    Msg8011F9B0 msg;
    Msg8011F9B0 *p = &msg;

    p->type_0 = 0xB;
    p->arg_4 = arg;
    p->field_10 = -1;
    target->vtable_24->func_5C((u8 *)target + target->vtable_24->delta_58, p);
}
