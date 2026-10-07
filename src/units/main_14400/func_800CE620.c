#include "common.h"

typedef unsigned char u8;

/* List prefix; func_800CE658 initializes the remaining container storage. */
typedef struct {
    void *pool;
    void *vtable;
} Object_800CE620;

extern u8 D_80154390[];
void func_800CE658(Object_800CE620 *obj, u8 *buffer, u8 capacity);

Object_800CE620 *func_800CE620(Object_800CE620 *obj, u8 *buffer, u8 capacity) {
    obj->vtable = D_80154390;
    func_800CE658(obj, buffer, capacity);
    return obj;
}
