#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern u8 func_800E8B10(void *,Item **);
u16 func_800EB6A0(void *arg,u16 value) {
    Item *items[2];
    s32 i;
    if(!value) return 0;
    i=func_800E8B10(arg,items);
    for(;;) {
        Item *item;
        if(--i==-1) break;
        item=items[i];
        switch(item->field01) {
        case 0x7F: return 0;
        case 0x7B: case 0x80: value*=2; break;
        }
    }
    return value;
}
