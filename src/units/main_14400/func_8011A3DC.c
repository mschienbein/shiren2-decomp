#include "common.h"

typedef struct VTable8011A3DC VTable8011A3DC;

typedef struct {
    char pad0[8];
    VTable8011A3DC *vtable;
} Obj8011A3DC;

extern VTable8011A3DC D_80153AA0;

extern void func_800AC68C(void *a);

/* Destructor: restore this class's vtable, free the storage when bit 0 is set. */
void func_8011A3DC(Obj8011A3DC *self, s32 flags) {
    self->vtable = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}
