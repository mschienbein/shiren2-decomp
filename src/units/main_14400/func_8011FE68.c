#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj_8011FE30;

extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj_8011FE30 *func_8011FE30(Obj_8011FE30 *obj);

/* Item factory (table D_80157964 slot 0): allocate 12 bytes and construct. */
Obj_8011FE30 *func_8011FE68(void) {
    return func_8011FE30(func_800AC5B4(0xC, 0));
}
