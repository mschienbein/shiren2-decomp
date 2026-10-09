#include "common.h"
typedef unsigned char u8;
typedef struct Msg { struct Msg *next; s32 field_4; short field_8; short field_A; s32 field_C; s32 field_10; } Msg;
typedef struct { u8 pad_0[0x88]; s32 field_88; } Node;
typedef struct { u8 pad_0[8]; Node *field_8; } Object;
typedef struct { u8 pad_0[0x1C]; s32 field_1C; } Clock;
extern Clock *D_80148D84;
extern Msg *func_80130780(void);
extern s32 func_8012E5F8(Node *, s32, Msg *);
void func_80130020(Object *object, u8 value) { u8 adjusted = value; Msg *message; if (object->field_8) { message = func_80130780(); if (message) { s32 time = D_80148D84->field_1C + object->field_8->field_88; message->field_8 = 0x10; message->field_4 = time; if ((signed char)value < 0) adjusted = 0x7F; message->field_C = adjusted; message->next = 0; func_8012E5F8(object->field_8, 3, message); } } }
