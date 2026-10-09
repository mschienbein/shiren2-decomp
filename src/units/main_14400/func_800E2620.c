#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0x72]; u8 field_72; } Object;
void func_800E2620(Object *object) { object->field_72 |= 4; }
