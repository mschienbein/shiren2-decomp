#include "common.h"
typedef struct { char pad0[4]; unsigned char field4; } Obj;
typedef struct { char pad0[0x30]; short adjust30; short pad32; void (*call34)(void *, void *); } VTable;
typedef struct Member { char pad0[4]; VTable *vtable; } Member;
s32 func_800CD278(Member *);
s32 func_800CD5C0(void *self, Obj *obj) { Member *member = self; if (func_800CD278(member) < obj->field4) return 0; member->vtable->call34((char *)member + member->vtable->adjust30, obj); return 1; }
