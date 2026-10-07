#include "common.h"
typedef struct { unsigned char pad_00[0x30]; short field_30; unsigned char pad_32[0x2CA]; s32 field_2FC; } Object;
typedef struct { unsigned char pad_00[5]; signed char field_05; } Entry;
extern s32 func_80098E34(Object *object, s32 value);
extern Entry *func_800980F0(Object *object, s32 index);
extern s32 func_80098F54(Object *object, s32 index);
extern void func_800514F0(Entry *entry, unsigned char *output, s32 mode, s32 scale, s32 offset, s32 kind, s32 flags);
void func_80047098(Object *object, unsigned char *output, s32 value, s32 flags) {
    s32 index;
    Entry *entry;
    *output = 0;
    index = func_80098E34(object, value);
    if (index >= 0) {
        entry = func_800980F0(object, index);
        if (entry) {
            s32 kind = 0;
            if (object->field_2FC == 0x100) kind = 2;
            else if (object->field_2FC != 0x400) {
                s32 present = entry->field_05 != -1;
                if (present) kind = 1;
            }
            func_800514F0(entry, output, 7, object->field_30 << 3,
                         value - func_80098F54(object, index), kind, flags);
        }
    }
}
