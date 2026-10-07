#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x29]; u8 unk29; } Obj8011541C;
void func_800B0B10(s32 id);
void func_8011541C(Obj8011541C *obj) {
    func_800B0B10(obj->unk29);
    obj->unk29 = 0;
}
