#include "common.h"
typedef short s16;
typedef struct { char pad[0xA8]; s16 offsetA8; s16 fieldAA; void (*callAC)(void *); } Table;
typedef struct { char pad[0x24]; Table *field24; } Obj;
extern s32 func_800A8FC8(s32 *, s32);
extern Obj *func_800A910C(s32 *);
void func_800C7A40(void) { s32 iterator = 0; for (;;) { Obj *p; Table *t; if (!func_800A8FC8(&iterator, 12)) break; p = func_800A910C(&iterator); t = p->field24; t->callAC((char *)p + t->offsetA8); } }
