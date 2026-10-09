#include "common.h"
/* Event 0x12: source unit at +4 and +0x1C, target unit at +8, direction byte at +0xC. */
typedef struct { s32 field_00; void *field_04; void *field_08; unsigned char field_0C; unsigned char field_0D[0xF]; void *field_1C; } Message;
/* Child table at +8, slot +0x38/+0x3C: message handler s32 (void *receiver, void *event)
 * (decided contract); the result is not needed here. */
typedef struct { unsigned char field_00[0x38]; short field_38; s32 (*field_3C)(void *receiver, void *event); } VTable;
typedef struct { unsigned char field_00[8]; VTable *field_08; } Child;
typedef struct { unsigned char field_00[0x10]; Child *field_10; } Object;
static inline void init_message(Message *message, void *value, unsigned char *data, void *other) { message->field_00 = 0x12; message->field_04 = value; message->field_08 = other; message->field_0C = *data; message->field_1C = value; }
void func_800C4C54(Object *object, void *value, unsigned char *data, void *other) { Message message; init_message(&message, value, data, other); object->field_10->field_08->field_3C((char *)object->field_10 + object->field_10->field_08->field_38, &message); }
