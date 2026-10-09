#include "common.h"
typedef struct { unsigned char pad0[0x28]; short adjust_28; short pad2A; void (*call_2C)(void *, s32, void *); } VTable;
typedef struct { unsigned char pad0[0x18]; VTable *field_18; } Obj;
struct Value;
extern struct Value D_80148780;
extern const char D_8015F984[];
void func_800CA4E8(Obj *self, void *value);
void func_80122820(Obj *self) {
    VTable *table;
    func_800CA4E8(self, (void *)D_8015F984);
    table = self->field_18;
    table->call_2C((unsigned char *)self + table->adjust_28, 9, &D_80148780);
}
