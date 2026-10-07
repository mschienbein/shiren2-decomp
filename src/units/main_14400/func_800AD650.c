#include "common.h"
extern unsigned char D_80147620[];
extern unsigned char D_80143084[];
extern void func_800C5634(void *object, void *value);
extern void func_800AD550(s32 value);
void func_800AD650(void) {
    func_800C5634(D_80147620, D_80143084);
    func_800AD550(1);
    func_800AD550(2);
    func_800AD550(6);
    func_800AD550(7);
    func_800AD550(9);
    func_800AD550(10);
}
