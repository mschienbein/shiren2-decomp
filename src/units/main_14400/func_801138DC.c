#include "common.h"

typedef struct { unsigned char pad[0xC]; unsigned char fC; } Timer;
void func_8011391C(Timer *t, void *entity);
void func_801138DC(Timer *t, void *entity) {
    if (t->fC != 0) {
        t->fC--;
        if (t->fC == 0) {
            func_8011391C(t, entity);
        }
    }
}
