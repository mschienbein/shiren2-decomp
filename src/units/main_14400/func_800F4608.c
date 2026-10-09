#include "common.h"
typedef struct { unsigned char fields00[0x0C]; unsigned short field0C; unsigned short field0E; } Entry;
typedef struct { unsigned char fields00[0x10]; Entry *field10; } Context;
typedef struct { unsigned char fields00[0x7E]; unsigned short field7E; } Object;
extern unsigned short func_800E0ED0(Object *object);
extern short func_800A0014(short value, unsigned short amount);
extern void func_800E3884(Object *object, Entry *entry, s32 value);
static inline Entry *get_entry(Context *context) {
    return context->field10;
}
void func_800F4608(Object *object, Context *context) {
    unsigned short value;
    Entry *entry;
    entry = get_entry(context);
    value = entry->field0C;
    if (!(entry->field0E & 8)) {
        value = func_800A0014((short)value, object->field7E + func_800E0ED0(object));
    }
    func_800E3884(object, entry, (short)value);
}
