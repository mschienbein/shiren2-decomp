#include "common.h"
typedef struct { unsigned char pad[8]; unsigned char field8; } Object;
typedef struct { unsigned char value; } Dir;
extern void *func_800A2594(void *, Object *, Dir);
void *func_800A6CC0(void *obj, Object *source) {
    Dir direction;
    direction.value = source->field8;
    func_800A2594(obj, source, direction);
    return obj;
}
