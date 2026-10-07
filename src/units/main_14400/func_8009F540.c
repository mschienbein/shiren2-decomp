#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 pad0[0xBB];
    s8 slots[12];
} Party;

typedef struct {
    u8 pad0[0x54];
    s8 members[12];
} Menu;

typedef struct {
    u8 pad0[0x3];
    u8 field_3;
} Request;

typedef struct {
    u8 field_0;
    u8 field_1;
    u8 field_2;
    u8 count;
} MenuArgs;

extern Party *func_800C9E00(void);
extern void func_8009D910(Menu *menu, Request *req, MenuArgs *args);

void func_8009F540(Menu *menu, Request *req)
{
    MenuArgs args;
    s32 count = 0;
    s32 i;
    s32 member;

    i = 0;
    while (1) {
        if (i >= 12) {
            break;
        }
        member = func_800C9E00()->slots[i];
        if (member != -1) {
            menu->members[count] = member;
            count++;
        }
        i++;
    }
    args.field_0 = req->field_3;
    args.field_1 = 1;
    args.count = count;
    func_8009D910(menu, req, &args);
}
