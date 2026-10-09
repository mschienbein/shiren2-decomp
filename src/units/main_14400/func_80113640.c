#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00; u8 field_01; u8 pad_02[10]; u8 field_0C; } Timer;
extern u8 D_80147620[], D_80156931;
extern u8 func_800C57A0(void *rng);
extern void func_8011391C(Timer *timer, void *entity);
extern void func_801137CC(Timer *timer, void *entity, u8 chance);
/* Original lbu 1(a0) proves the first argument is an object pointer. */
void func_80113640(Timer *timer, void *entity) {
    if (timer->field_01 == 0x8D) {
        if (timer->field_0C != 0) {
            if ((u32)func_800C57A0(D_80147620) < 0x29U) func_8011391C(timer, entity);
        } else func_801137CC(timer, entity, D_80156931);
    }
}
