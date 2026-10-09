#include "common.h"

typedef struct { s32 field00; s32 unknown04[3]; s32 field10; s32 unknown14; } Message;
/* Actor message handler slot +0x58/+0x5C returns a status word: func_800C4D58/func_800C4E80
 * return it to func_800C4864, which tests it at 0x800C49D4. It is ignored here. */
typedef struct { unsigned char unknown00[0x58]; short adjust58; short unknown5a; s32 (*method5c)(void *, Message *); } VTable;
typedef struct { s32 unknown00; s32 field04; } Component;
typedef struct { unsigned char unknown00[0x1e]; unsigned char field1e; unsigned char unknown1f[5]; VTable *field24; unsigned char unknown28[0x5c]; Component field84; } Object;
typedef struct { s32 field00; s32 unknown04; } Iterator;
extern s32 func_800A8FC8(Iterator *, s32);
extern Object *func_800A910C(Iterator *);
void func_800C78F8(s32 value) {
    Message message;
    Iterator iterator;
    message.field00 = 5;
    message.field10 = value;
    iterator.field00 = 0;
    while (func_800A8FC8(&iterator, 0x7c)) {
        Object *object = func_800A910C(&iterator);
        s32 skip = 0;
        if ((object->field1e >> 3) & 1) {
            Component *component = 0;
            if (object) component = &object->field84;
            if (!component->field04) skip = 1;
        }
        if (!skip) {
            VTable *table = object->field24;
            table->method5c((char *)object + table->adjust58, &message);
        }
    }
}
