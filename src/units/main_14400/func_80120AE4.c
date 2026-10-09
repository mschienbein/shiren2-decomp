#include "common.h"

typedef struct Obj Obj;

void func_8011414C(Obj *self, s32 flags);
void func_800AC68C(void *a);

/* Destructor: run the base destructor without freeing, then free if asked. */
void func_80120AE4(Obj *self, s32 flags) {
    func_8011414C(self, 0);
    if (flags & 1) {
        func_800AC68C(self);
    }
}
