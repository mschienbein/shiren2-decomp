#include "common.h"
typedef struct { unsigned char pad0[8]; void *vtable8; s32 fieldC; } Object;
extern unsigned char D_8015FC60[];
extern void *func_8010E020(void *object, s32 kind);
extern unsigned short func_800AB044(void);
Object *func_80123990(Object *object) {
    func_8010E020(object, 0xCC);
    object->vtable8 = D_8015FC60;
    object->fieldC = func_800AB044();
    return object;
}
