#include "common.h"
typedef struct { s32 x, y; } Value;
typedef Value Position;
typedef struct { unsigned char pad0[0x18]; short adjust_18; short pad1A; void (*call_1C)(void *); } VTable;
typedef struct { unsigned char pad0[0x24]; VTable *field_24; } Object;
s32 func_800A5D2C(void *self, Value *out, s32 flags);
void func_800A58FC(void *self, Position *position);
s32 func_800A5DC4(Object *self, Value *position, s32 flags) {
    s32 result;
    if (!func_800A5D2C(self, position, flags)) {
        VTable *table = self->field_24;
        table->call_1C((unsigned char *)self + table->adjust_18);
        result = 0;
    } else {
        func_800A58FC(self, position);
        result = 1;
    }
    return result;
}
