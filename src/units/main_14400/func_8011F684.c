#include "common.h"

typedef unsigned char u8;

extern u8 D_80153AA0[];
extern void func_800AC68C(void *obj);

typedef struct {
    s32 field0;
    s32 field4;
    void *vtable8;
} Obj8011F684;

void func_8011F684(Obj8011F684 *obj, s32 flags) {
    obj->vtable8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
