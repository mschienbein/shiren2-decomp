#include "common.h"

typedef struct { char pad[0x20]; short offset20; short pad22; s32 (*method24)(void *); } VTable;
typedef struct { s32 field0; VTable *vtable; } Child;
typedef struct { char pad[0x8C]; Child *child; } Object;
s32 func_800F3920(Object *obj) { Child *child=obj->child; return child->vtable->method24((char *)child+child->vtable->offset20) != 0; }
