#include "common.h"
typedef unsigned char u8;
typedef struct { u32 capacity; u32 count; u32 field_08; void *records; } Object;
extern void *func_80091450(u32 bytes);
extern u8 *func_8006A810(void *dst, s32 value, s32 count);
s32 func_8008D40C(Object *self, u32 capacity) {
    u32 bytes = capacity * 8U;
    s32 result;
    self->records = func_80091450(bytes);
    if (self->records) {
        func_8006A810(self->records, 0, bytes);
        self->capacity = capacity;
        self->count = 0;
        self->field_08 = 0;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}
