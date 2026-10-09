#include "common.h"

typedef struct { s32 x, y; } Position;
typedef struct { unsigned char pad_00[8]; Position position_08; } Object;
extern s32 func_800AD8AC(void *, void *);

void func_800CFC08(Object *object, void *value) {
    func_800AD8AC(value, &object->position_08);
}
