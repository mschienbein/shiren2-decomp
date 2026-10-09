#include "common.h"
typedef unsigned short u16;
typedef struct { char pad[0x9A]; u16 field9A; } Obj;
typedef struct { s32 field0; s32 field4; } Point;
extern Obj *D_801476B8;
extern Obj *func_801217DC(void *);
extern char *func_800A3B20(Obj *);
extern char *func_800A3CD0(Obj *);
extern void func_800498E4(s32, ...);
extern Point *func_800A65E4(Point *, Obj *, Obj *);
extern void func_800A6690(Obj *, Point *, s32);
static inline void move_object(Obj *p, Point *point) { func_800A65E4(point, p, D_801476B8); func_800A6690(p, point, 1); }
void func_80121B04(void *owner) { Obj *p = func_801217DC(owner); char *name = func_800A3B20(D_801476B8); Point point; func_800498E4(107, name, func_800A3CD0(p)); move_object(p, &point); p->field9A |= 0x80; }
