#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x8];
    void *vtable_08;
} Obj80112470;

/* Base constructor: initializes the object with a kind and an argument; returns it. */
extern Obj80112470 *func_8010C8C0(Obj80112470 *obj, s32 kind, s32 arg);
extern void func_801124B4(Obj80112470 *obj);

/* Initialized original vtable; its full type is unresolved. */
extern u8 D_8015D650[];

Obj80112470 *func_80112470(Obj80112470 *obj, s32 arg)
{
    func_8010C8C0(obj, 5, arg);
    obj->vtable_08 = D_8015D650;
    func_801124B4(obj);
    return obj;
}
