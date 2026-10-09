#include "common.h"

typedef unsigned char u8;

/* Partial view of the target: kind at 0x00, pending flag at 0x28. */
typedef struct Item8011B850 {
    u8 kind_00;
    u8 pad_01[0x27];
    u8 pending_28;
} Item8011B850;

s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);
char *func_800AE674(void *obj);

/* Virtual method (vtable D_8015E840 slot 0x44): the receiver and position are unused. */
void func_8011B850(void *self, void *pos, Item8011B850 *target)
{
    if (target != 0 && target->kind_00 == 9) {
        if (target->pending_28 != 0) {
            target->pending_28 = 0;
            func_80049CB4(0x2E);
            func_800498E4(0xD8, func_800AE674(target));
        } else {
            func_80049CB4(0x132);
            func_800498E4(0x222);
        }
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x223);
    }
}
