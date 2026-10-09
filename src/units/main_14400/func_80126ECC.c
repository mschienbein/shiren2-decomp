#include "common.h"

typedef struct {
    s32 fields_00[2];
    void *table_08;
} Object;

extern s32 D_80153AA0[];
extern void func_800AC68C(void *);

void func_80126ECC(Object *object, s32 flags) {
    object->table_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
