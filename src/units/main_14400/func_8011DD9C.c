#include "common.h"

typedef unsigned char u8;

/* Message passed to slot 0x3C of the item class (same layout as func_800A00C4's). */
typedef struct Message8011DD9C {
    s32 type_00;
    void *source_04;
    void *target_08;
    u8 kind_0C;
    u8 pad_0D[3];
    u8 pad_10[8];
    s32 value_18;
    void *attacker_1C;
} Message8011DD9C;

typedef struct Methods8011DD9C {
    u8 pad_00[8];
    short delta_08;
    short index_0A;
    void (*destroy_0C)(void *self, s32 flags);
    u8 pad_10[0x38 - 0x10];
    short delta_38;
    short index_3A;
    s32 (*handle_3C)(void *self, Message8011DD9C *msg);
} Methods8011DD9C;

typedef struct Child8011DD9C {
    u8 pad_00[8];
    Methods8011DD9C *vtable_08;
} Child8011DD9C;

/* Partial view: only the child id at 0x10 is used. */
typedef struct Obj8011DD9C {
    u8 pad_00[0x10];
    u8 child_10;
} Obj8011DD9C;

void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
Child8011DD9C *func_800D8BB0(Obj8011DD9C *object, s32 value);

/* Virtual method (slot 0x54 of D_8015ED90 / D_80149030 family). */
void func_8011DD9C(Obj8011DD9C *self, void *source, void *target, u8 *kind, void *attacker)
{
    Child8011DD9C *child;
    Message8011DD9C msg;
    Message8011DD9C *m;

    if (self->child_10 != 0) {
        child = func_800D8BB0(self, self->child_10);
        if (child != 0) {
            m = &msg;
            m->type_00 = 0x13;
            m->source_04 = source;
            m->target_08 = target;
            m->kind_0C = *kind;
            m->value_18 = 0;
            m->attacker_1C = attacker;
            child->vtable_08->handle_3C((u8 *)child + child->vtable_08->delta_38, m);
            child->vtable_08->destroy_0C((u8 *)child + child->vtable_08->delta_08, 3);
        }
    } else {
        func_800A7B18(target, source, 1, 6);
    }
}
