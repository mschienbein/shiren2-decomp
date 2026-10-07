#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 a, b; } Pair;
typedef struct {
    u8 kind;
    u8 x1;
    u8 pad2[3];
    s8 x5;
    u8 pad6[6];
    u8 xC;
} Item;
char *func_800AE674(Item *item);
s32 func_8010BEC4(Item *item, u8 id);
s32 func_8010BD00(Item *item, u8 id);
u16 func_800AE710(Item *item);
void *func_800AC5F4(s32 size, Item *item);
Item *func_8011D400(void *mem);
void func_800AE6C4(Item *item, u16 count);
void func_800AE974(Item *item, s8 value);
void func_80113914(Item *item);
s32 func_80049CB4(s32 id, ...);
void func_80049A04(u16 message_id, ...);
void func_800498E4(s32 message_id, ...);
/* Item-effect slot +0x44 supplies self, actor and item; this override ignores self. */
void func_8011A940(void *unused, Pair *who, Item *item) {
    if (item != 0) {
        char *name = func_800AE674(item);
        switch (item->kind) {
        case 3:
        case 4:
            if ((u8)func_8010BEC4(item, 0x1D)) {
                func_80049CB4(0x132);
                func_80049A04(0xD3, name);
            } else if (func_8010BD00(item, 0x1D)) {
                func_80049CB4(0x2C, who, item);
                func_80049A04(0xD2, name);
            } else {
                func_80049CB4(0x132);
                func_80049A04(0xD4, name);
            }
            return;
        case 6:
            if (item->xC) {
                func_80049CB4(0x2C, who, item);
                func_80113914(item);
                func_80049A04(0xD5, func_800AE674(item));
                return;
            }
            break;
        default:
            if (item->x1 == 0x77) {
                u16 count = func_800AE710(item);
                s32 x5 = item->x5;
                Pair tmp;
                item = func_8011D400(func_800AC5F4(0x18, item));
                func_800AE6C4(item, (u32)(count + 1) >> 1);
                func_800AE974(item, x5);
                {
                    Pair *dst = &tmp;
                    dst->a = who->a;
                    dst->b = who->b;
                }
                func_80049CB4(0x11D, &tmp);
                func_80049A04(0x229, name, func_800AE674(item));
                return;
            }
            break;
        }
    }
    func_80049CB4(0x132);
    func_800498E4(0x223);
}
