#include "common.h"

/* Slot +0x94 targets func_800E115C / derived dispatchers; +0x90 is zero in their tables. */
typedef struct { unsigned char unknown00[0x90]; short adjust90; short unknown92; s32 (*method94)(void *, s32, s32, unsigned char, s32); } VTable;
typedef struct { unsigned char unknown00[0x24]; VTable *field24; } Object;
s32 func_800E2770(Object *object) {
    return object->field24->method94((char *)object + object->field24->adjust90, 0, 0x13, 0xfe, 0);
}
