#include "common.h"
#include "item_records.h"
typedef struct { s32 field_0; s32 field_4[3]; s32 field_10, field_14; } Message;
extern unsigned char D_80156A79;
extern s32 func_80049CB4(s32 command, ...);
extern void func_800A00C4(void *, unsigned char, void *, s32);
extern s32 func_800AF28C(Item *, Message *);
typedef struct { s32 x, y; } Point;
static inline Point *copy_point(Point *point, Message *message) { point->x = message->field_10; point->y = message->field_14; return point; }
s32 func_80128FB0(Item *self, Message *message) { s32 result; if (message->field_0 == 0x1B) { Point point; Point *position = copy_point(&point, message); func_80049CB4(0xF6, position); func_800A00C4(position, D_80156A79, 0, 0x14); if (self) self->field08->method0C((unsigned char *)self + self->field08->delta08, 3); result = 1; } else result = func_800AF28C(self, message); return result; }
