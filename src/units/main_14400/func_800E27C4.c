#include "common.h"
typedef struct { char pad0[0x90]; short adjust90; short pad92; s32 (*call94)(void *, s32, s32, unsigned char, s32); } VTable;
typedef struct { char pad0[0x24]; VTable *vtable; } Obj;
s32 func_800E27C4(Obj *obj) { return obj->vtable->call94((char *)obj + obj->vtable->adjust90, 1, 0x11, 0, 0); }
