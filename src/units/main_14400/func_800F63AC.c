#include "common.h"
typedef unsigned char u8;
typedef short s16;
/* D_801599F8 + 0x64 resolves to the void refresh method func_800F62F0. */
typedef struct { u8 pad0[0x60]; s16 delta60; s16 pad62; void (*method64)(void *); } VTable;
typedef struct { u8 pad0[0x24]; VTable *vtable; } Object;
extern s16 func_800F3E68(Object *object, s16 amount, u8 maximum);
void func_800F63AC(Object *object, s16 amount) {
    func_800F3E68(object, amount, 3);
    object->vtable->method64((char *)object + object->vtable->delta60);
}
