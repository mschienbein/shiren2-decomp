#include "common.h"
extern s32 D_8016FD50, D_8016FD54;
void func_8006A270(s32 change, s32 capacity_change) {
    s32 value = D_8016FD50 + change;
    s32 current = value < 0 ? 0 : (D_8016FD54 < value ? D_8016FD54 : value);
    s32 capacity;
    D_8016FD50 = current;
    capacity = D_8016FD54 + capacity_change;
    if (capacity >= current) {
        current = 1000;
        if (capacity < 1001) current = capacity;
    }
    D_8016FD54 = current;
}
