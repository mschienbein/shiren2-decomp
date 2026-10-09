#include "common.h"

typedef struct {
    s32 fields_00[9];
    void *table_24;
    s32 fields_28[22];
    s32 member_80;
} Object;

/* Base constructor returns its receiver; this derived constructor ignores it. */
extern Object *func_800E0120(Object *);
extern void func_80136908(void *);
extern s32 func_800A3934(Object *);
extern void func_800E8784(Object *);
extern s32 D_80158E80[];

Object *func_800E8730(Object *object) {
    func_800E0120(object);
    object->table_24 = D_80158E80;
    func_80136908(&object->member_80);
    if (func_800A3934(object) == 0) {
        func_800E8784(object);
    }
    return object;
}
