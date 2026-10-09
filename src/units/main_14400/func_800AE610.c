#include "common.h"
/* Object table at +8, slot +0x38/+0x3C: message handler s32 (void *receiver, void *event)
 * (decided contract); the result is not needed here. */
typedef struct { unsigned char field_0[0x38]; short field_38; s32 (*field_3C)(void *receiver, void *event); } VTable;
typedef struct { s32 field_0[2]; VTable *field_8; } Object;
/* Event 0x10 consumers func_80120D88 and func_8010D83C interpret +4 as an
 * actor pointer; func_800ED5FC supplies its own object address here. */
typedef struct { s32 kind; void *actor; unsigned char payload[0x18]; } Message;
void func_800AE610(Object *arg, void *value) { Message message; message.kind = 0x10; message.actor = value; arg->field_8->field_3C((unsigned char *)arg + arg->field_8->field_38, &message); }
