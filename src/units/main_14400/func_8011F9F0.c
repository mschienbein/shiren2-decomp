#include "common.h"

typedef struct {
    char pad0[8];
    void *unk8;
} Obj8011F9F0;

extern char D_80153AA0[];
void func_800AC68C(Obj8011F9F0 *obj);

void func_8011F9F0(Obj8011F9F0 *obj, s32 flags) {
    obj->unk8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
