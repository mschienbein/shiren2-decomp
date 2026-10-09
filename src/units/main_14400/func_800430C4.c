#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { short adjust; u16 reserved; void (*call)(void *); } Entry;
typedef struct { s32 field0, field4; Entry *field8; s32 fieldC; } Object;
typedef struct { u16 field0, field2; s32 field4; Pair *field8; s32 fieldC, field10; } Group;
typedef struct { s32 field0; Group *field4; } Groups;
typedef struct { s32 field0; Groups *field4; } Root;
typedef struct { u8 pad[0xC]; u16 fieldC; } Item;
extern u16 D_80143450[];
extern Root *func_80066B20(void);
extern void func_800B1080(void);
extern void *func_800AC5B4(s32, s32);
extern Item *func_8010DD70(void *, s32);
extern s32 func_800AC670(Item *);
extern s32 func_800AD8AC(Item *, Pair *);
extern s32 func_80049CB4(s32, ...);
static inline void swap_pair(Pair *dest, const Pair *source) {
    s32 x = source->y, y = source->x;
    dest->x = x;
    dest->y = y;
}
void func_800430C4(Object *obj) {
    Root *root = func_80066B20();
    s32 i, j;
    u16 *cursor;
    Pair position;
    func_800B1080();
    cursor = D_80143450;
    {
        s32 count;
        for (count = 0x1007; count >= 0; count--) *cursor++ = 0;
    }
    obj->field8[2].call((u8 *)obj + obj->field8[2].adjust);
    if (!obj->field4) {
        for (i = 0;; i++) {
            Group *group;
            if (i >= root->field4->field0) break;
            if (obj->fieldC >= 0 && i != obj->fieldC) continue;
            group = &root->field4->field4[i];
            for (j = 0;; j++) {
                Item *item;
                if (j >= group->field4) break;
                item = func_8010DD70(func_800AC5B4(0x10, 1), 0xCE);
                item->fieldC = group->field2;
                if (func_800AC670(item)) continue;
                swap_pair(&position, &group->field8[j]);
                func_800AD8AC(item, &position);
            }
        }
    }
    func_80049CB4(5, 1);
}
