#include "common.h"

typedef struct {
    char data[0x2C];
} Obj_80120868;

extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj_80120868 *func_80120830(Obj_80120868 *obj);

Obj_80120868 *func_80120868(void) {
    return func_80120830(func_800AC5B4(sizeof(Obj_80120868), 0));
}
