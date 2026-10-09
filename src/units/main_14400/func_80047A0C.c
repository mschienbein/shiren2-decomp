#include "common.h"
typedef struct { char pad0[0x4C]; const void *field4C; } Base;
/* Partial containing-object view through the last vtable at +0x2A0. */
typedef struct {
    char pad0[0x4C];
    const void *field4C;
    char pad50[0x78];
    Base memberC8;
    char pad118[0x7C];
    const void *field194;
    char pad198[0x108];
    const void *field2A0;
} Obj;
/* The nested receiver begins at +0xC8; keep its +0x60 child in the parent view. */
extern char D_8014A9E8[];
extern const unsigned char D_80151E38[144];
extern void func_800951D0(void *, s32);
extern void func_800D8FA8(Obj *);
static inline void destroy_member(Obj *object, Base *member) { func_800951D0((char *)object + 0x128, 2); member->field4C = D_80151E38; }
void func_80047A0C(Obj *p, s32 flags) { p->field4C = D_8014A9E8; p->field2A0 = D_80151E38; p->field194 = D_80151E38; destroy_member(p, &p->memberC8); p->field4C = D_80151E38; if (flags & 1) func_800D8FA8(p); }
