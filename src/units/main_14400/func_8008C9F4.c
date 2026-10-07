#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 active_0;
    u8 pad1[0x4F];
    void *items_50[9];
} Obj8008C9F4;
void *func_8008CED4(Obj8008C9F4 *obj);
void func_8008D3F0(void *item);
void func_80091544(void *item);

void func_8008C9F4(Obj8008C9F4 *obj) {
    s32 i;

    if (obj->active_0) {
        func_8008CED4(obj);
        obj->active_0 = 0;
        for (i = 0; i < 9; i++) {
            void *item = obj->items_50[i];
            if (item) {
                func_8008D3F0(item);
                item = obj->items_50[i];
                func_80091544(item);
                obj->items_50[i] = 0;
            }
        }
    }
}
