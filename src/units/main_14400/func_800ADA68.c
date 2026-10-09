#include "common.h"

typedef struct { s32 field00; s32 field04; } Position;
typedef struct { s32 field00; void *field04; s32 unknown08[2]; Position field10; s32 unknown18[2]; } Message;
typedef struct { unsigned char unknown00[0x18]; short adjust18; short unknown1a; s32 (*method1c)(void *, s32); unsigned char unknown20[0x18]; short adjust38; short unknown3a; s32 (*method3c)(void *, Message *); } VTable;
typedef struct { s32 unknown00[2]; VTable *field08; } Object;
extern char D_80147620[];
extern unsigned char D_80156929;
extern u32 func_800B1C6C(Position *);
extern s32 func_800C587C(void *, unsigned char);
static __inline__ s32 denied(void) {
    return func_800C587C(D_80147620, D_80156929) ^ 1;
}
s32 func_800ADA68(Object *object, void *value, Position *position, s32 mode) {
    Message message;
    s32 blocked = 0;
    VTable *table = object->field08;
    if (!table->method1c((char *)object + table->adjust18, 0x1d) || (func_800B1C6C(position) & 0x2000)) blocked = 1;
    if (blocked) return 0;
    if (mode == 1 && denied()) return 0;
    if (mode) value = 0;
    {
        Message *command = &message;
        message.field00 = 0xe;
        command->field04 = value;
        message.field10 = *position;
        table = object->field08;
        table->method3c((char *)object + table->adjust38, command);
    }
    return 1;
}
