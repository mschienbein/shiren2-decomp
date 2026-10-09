#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x58];
    u8 text58[0x10];
} Obj8009E4C8;

extern void func_80048728(void *);

/* Vtable slot 3 of D_80152D68. */
void func_8009E4C8(Obj8009E4C8 *self) {
    func_80048728(self->text58);
}
