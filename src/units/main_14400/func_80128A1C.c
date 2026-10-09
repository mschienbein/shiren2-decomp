#include "common.h"
typedef unsigned char u8;
typedef struct { short offset; short field_2; void (*method)(void *, s32, void *); } Method;
typedef struct { unsigned char field_0[0x18]; Method *field_18; } Context;
/* The stream saves the complete four-byte group at +0xC (func_80128A78 restores it). */
typedef struct { u8 field_C; u8 field_D; u8 field_E; u8 field_F; } Saved80128A1C;
typedef struct { u8 pad0[0xC]; Saved80128A1C fields_C; } Obj80128A1C;
extern u8 D_80160714[]; /* "Back" type tag */
extern void func_800AF11C(void *, Context *);
extern void func_800CA4A4(Context *, void *);
void func_80128A1C(Obj80128A1C *self, Context *context) { func_800AF11C(self, context); func_800CA4A4(context, D_80160714); context->field_18[3].method((unsigned char *)context + context->field_18[3].offset, 4, &self->fields_C); }
