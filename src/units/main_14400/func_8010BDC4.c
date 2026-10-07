#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00[0xF];
    u8 field_0F;
    u8 field_10[16];
} List;

s32 func_8010BDC4(List *list, u8 value) {
    s32 removed = 0;
    s32 index = list->field_0F;
    index--;
    for (; index != -1; index--) {
        if (list->field_10[index] == value) {
            s32 position;
            removed++;
            for (position = index; position < list->field_0F - 1; position++) {
                list->field_10[position] = list->field_10[position + 1];
            }
            list->field_10[position] = 0;
        }
    }
    list->field_0F -= removed;
    return removed != 0;
}
