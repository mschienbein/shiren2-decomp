#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x8];
    void *vtable_08;
} Obj80115570;

/* Base constructor: initializes the object with a kind and an argument; returns it. */
extern Obj80115570 *func_8010C8C0(Obj80115570 *obj, s32 kind, s32 arg);

/* Initialized original vtable; its full type is unresolved. */
extern u8 D_8015D988[];

/* Constructor: the original returns the constructed object in v0. */
Obj80115570 *func_80115570(Obj80115570 *obj, s32 arg)
{
    func_8010C8C0(obj, 0xD, arg);
    obj->vtable_08 = D_8015D988;
    return obj;
}
