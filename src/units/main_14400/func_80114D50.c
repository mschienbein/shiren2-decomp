#include "common.h"
typedef struct { unsigned char pad[0x10]; short field10; s32 (*field14)(void *); } Methods;
typedef struct { s32 field0; Methods *field4; } Position;
typedef struct { unsigned char field0, field1; unsigned char pad2[3]; signed char field5; unsigned char pad6[6]; Position fieldC; } Object;
extern s32 func_800CD278(Position *);
extern u32 func_80114B68(Object *);
extern void func_800D3698(s32, s32);
static inline s32 is_active(Object *p) { return ~p->field5 && p->field1 != 0xA6; }
void func_80114D50(Object *p, s32 x, s32 y) {
    if (is_active(p)) {
        Position *position = &p->fieldC;
        signed char distance = 0;
        Methods *methods = position->field4;
        s32 oldx = methods->field14((unsigned char *)position + methods->field10);
        s32 oldy = func_800CD278(position);
        signed char dx = x - oldx;
        signed char dy = y - oldy;
        s32 id;
        if (dx > 0) distance = dx;
        else if (dy > 0) distance = dy;
        id = p->field5;
        func_800D3698(id, (s32)(func_80114B68(p) * (u32)distance));
    }
}
