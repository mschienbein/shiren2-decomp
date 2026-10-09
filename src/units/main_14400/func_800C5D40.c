#include "common.h"

typedef struct {
    s32 fields_00[3];
    void *table_0C;
} Object;

extern s32 D_80149DC8[];
extern void func_800D8FA8(void *);

void func_800C5D40(Object *object, s32 flags) {
    object->table_0C = D_80149DC8;
    if (flags & 1) {
        func_800D8FA8(object);
    }
}
