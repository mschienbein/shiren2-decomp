#include "common.h"
extern unsigned char D_80165974;
extern s32 D_801DE984;
void func_8005CEDC(void);
void func_8005CE68(void);
unsigned char func_8005D24C(void **cursor);
void *func_8006EA64(void *cursor, void *end, void *pool);
void func_8005D700(void **p) {
    void *value = *p;
    if (D_80165974 == 0) return;
    if (D_80165974 & 0x20) {
        func_8005CEDC();
        func_8005CE68();
        value = func_8006EA64(value, 0, &D_801DE984);
    } else {
        func_8005CEDC();
        func_8005CE68();
        if (func_8005D24C(&value)) {
            value = func_8006EA64(value, 0, &D_801DE984);
        }
    }
    *p = value;
}
