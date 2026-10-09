#include "common.h"

typedef struct Obj8011414C Obj8011414C;

extern void func_8011414C(Obj8011414C *self, s32 flags);
extern void func_800AC68C(void *a);

/* Destructor: run the base destructor, free the storage when bit 0 is set. */
void func_801208BC(Obj8011414C *self, s32 flags) {
    func_8011414C(self, 0);
    if (flags & 1) {
        func_800AC68C(self);
    }
}
