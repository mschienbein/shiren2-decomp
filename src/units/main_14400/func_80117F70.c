#include "common.h"
typedef struct { char pad0[0x78]; short adjust78; short pad7A; void (*call7C)(void *, short); } VTable;
typedef struct { char pad0[0x24]; VTable *vtable; } Obj;
s32 func_800A99D0(void);
void func_800498E4(s32 id, ...);
s32 func_800E1CC4(Obj *obj, s32 kind);
extern const short D_801569D4;
/* Item vtable slot +0x44: `self` is the adjusted receiver the dispatcher supplies; not used here. */
void func_80117F70(void *self, Obj *obj) { if (func_800A99D0()) func_800498E4(0x222); else { s32 factor = func_800E1CC4(obj, 3) ? 2 : 1; obj->vtable->call7C((char *)obj + obj->vtable->adjust78, (short)(D_801569D4 * factor)); } }
