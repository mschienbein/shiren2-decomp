#include "common.h"
typedef struct { s32 field_00, field_04, field_08; unsigned char field_0C; unsigned char field_0D[0xF]; s32 field_1C; } Message;
typedef struct { unsigned char field_00[0x38]; short field_38; void (*field_3C)(void *, Message *); } VTable;
typedef struct { unsigned char field_00[8]; VTable *field_08; } Child;
typedef struct { unsigned char field_00[0x10]; Child *field_10; } Object;
static inline void init_message(Message *message, s32 value, unsigned char *data, s32 other) { message->field_00 = 0x12; message->field_04 = value; message->field_08 = other; message->field_0C = *data; message->field_1C = value; }
void func_800C4C54(Object *object, s32 value, unsigned char *data, s32 other) { Message message; init_message(&message, value, data, other); object->field_10->field_08->field_3C((char *)object->field_10 + object->field_10->field_08->field_38, &message); }
