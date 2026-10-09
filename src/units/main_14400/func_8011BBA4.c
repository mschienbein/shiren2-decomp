#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    s32 x;
    s32 y;
} Pair8011BBA4;

typedef struct {
    u8 pad0[0x10];
    Pair8011BBA4 pos;
} Obj8011BBA4;

void func_8011BBA4(Obj8011BBA4 *obj, Pair8011BBA4 *pos) {
    obj->pos = *pos;
}
