#include "common.h"
typedef struct Message { struct Message *next; s32 field_04; short field_08; short field_0A; float field_0C; } Message;
typedef struct { unsigned char field_00[0x88]; s32 field_88; } Child;
typedef struct { unsigned char field_00[8]; Child *field_08; } Object;
typedef struct { unsigned char field_00[0x1C]; s32 field_1C; } Owner;
extern Owner *D_80148D84;
extern Message *func_80130780(void);
extern s32 func_8012E5F8(Child *, s32, Message *);
void func_80130150(Object *object, float value) { Message *message; if (object->field_08) { message = func_80130780(); if (message) { s32 offset = D_80148D84->field_1C + object->field_08->field_88; message->field_08 = 7; message->field_0C = value; message->next = 0; message->field_04 = offset; func_8012E5F8(object->field_08, 3, message); } } }
