#include "common.h"
#include "item_records.h"

void func_8011391C(Item *t, void *entity);
void func_801138DC(Item *t, void *entity) {
    if (t->payload.timer.turns0C != 0) {
        t->payload.timer.turns0C--;
        if (t->payload.timer.turns0C == 0) {
            func_8011391C(t, entity);
        }
    }
}
