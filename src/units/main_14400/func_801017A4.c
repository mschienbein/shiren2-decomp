#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[4];
    u8 unk4;
} Item;

typedef struct {
    u8 pad0[0x28];
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
    u8 pad30[0x5C];
    void *unk8C;
    u8 pad90[0x18];
    u8 unkA8[0x18];
    u8 unkC0;
} Obj;

extern u8 D_80157010[];
extern u8 D_80157014[];
extern u8 D_80157018[];
extern s32 func_800CD278(void *container);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Obj *obj);
extern char *func_800AC9D8(Item *item);
extern void func_800498E4(s32 id, ...);
extern s32 func_800E0F40(Obj *obj);
extern s32 func_800CD61C(void *container, Item *item, s32 any);

s32 func_801017A4(Obj *obj, Item *item) {
    s32 enough = (u32)func_800CD278(obj->unk8C) >= item->unk4;
    u16 percent;

    if (enough) {
        char *name;
        func_80049CB4(0x1131);
        func_80049CB4(6);
        func_80049CB4(0x68, obj);
        func_80049CB4(7);
        name = func_800A3B20(obj);
        func_800498E4(0x143, name, func_800AC9D8(item));
        switch ((u8)func_800E0F40(obj)) {
        case 1:
            percent = D_80157010[obj->unkC0];
            percent += 100;
            break;
        case 2:
            percent = D_80157014[obj->unkC0];
            percent += 100;
            break;
        default:
            percent = D_80157018[obj->unkC0];
            percent += 100;
            break;
        }
        obj->unk2A = obj->unk2A * percent / 100;
        obj->unk28 = obj->unk28 * percent / 100;
        obj->unk2C = obj->unk2C * percent / 100;
        obj->unk2E = obj->unk2E * percent / 100;
        func_800CD61C(obj->unkA8, item, 1);
        {
            u8 n = item->unk4;
            obj->unkC0 += n;
        }
        return 1;
    }
    return 0;
}
