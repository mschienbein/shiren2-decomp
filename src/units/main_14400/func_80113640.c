#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
extern u8 D_80147620[], D_80156931;
extern u8 func_800C57A0(void *rng);
extern void func_8011391C(Item *timer, void *entity);
extern void func_801137CC(Item *timer, void *entity, u8 chance);
/* Original lbu 1(a0) proves the first argument is an object pointer. */
void func_80113640(Item *timer, void *entity) {
    if (timer->field01 == 0x8D) {
        if (timer->payload.timer.turns0C != 0) {
            if ((u32)func_800C57A0(D_80147620) < 0x29U) func_8011391C(timer, entity);
        } else func_801137CC(timer, entity, D_80156931);
    }
}
