#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

void *func_800EB7C4(u8 *obj);
u16 func_800AE710(void *item);
void func_800AE6C4(void *item, u16 count);
void func_800CD364(void *list, void *item);
void func_800EB8C8(u8 *obj, s32 amount) {
    void *item = func_800EB7C4(obj);
    s32 count;
    if (item != 0) {
        count = func_800AE710(item);
        if (count <= amount) {
            func_800CD364(obj + 0xCC, item);
        } else {
            func_800AE6C4(item, count - amount);
        }
    }
}
