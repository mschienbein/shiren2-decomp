#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 field_0; u8 field_1; u8 field_2; } Item;
extern u8 D_80143393;
void *func_800B51D4(void *object);
s32 func_800B5690(void *object) {
    Item *item = func_800B51D4(object);

    if (item != 0) {
        if (item->field_2 != 0) {
            if (item->field_2 <= 100) {
                D_80143393--;
            }
            item->field_2 = 0;
            return 1;
        }
        return 0;
    }
    return 0;
}
