#include "common.h"
typedef struct { unsigned char pad0[8]; void *field_8; } Obj80123430;
extern Obj80123430 *func_8010C8C0(Obj80123430 *obj, s32 kind, s32 value);
extern unsigned char D_8015D398[];
Obj80123430 *func_8010E5C0(Obj80123430 *obj, s32 arg1) {
    func_8010C8C0(obj, 11, arg1);
    obj->field_8 = D_8015D398;
    return obj;
}
