#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    unsigned char unk0;
    unsigned char pad1;
    unsigned char unk2;
} Item;

typedef struct {
    unsigned char pad0[8];
    unsigned char unk8;
} Obj;

typedef struct {
    unsigned char dir;
} Turn;

extern void *func_800E8978(Obj *obj);
extern void *func_800A6CC0(Pos *pos, Obj *obj);
extern s32 func_800A4754(Obj *obj, Obj *other, unsigned char *dir);
extern s32 func_800B4888(Pos *pos);
extern s32 func_800E1CC4(Obj *obj, s32 kind);
extern void func_800A665C(Obj *obj, Turn *turn);
extern void func_800AE610(void *arg0, Obj *obj);
extern void func_800E8FAC(Obj *obj);
extern Item *func_800B4D80(Pos *pos);
extern s32 func_800EC630(Obj *obj, Item *item);
extern s32 func_80049CB4(s32 id, ...);

static __inline__ s32 itemFlag10Set(Item *item) {
    unsigned char flags = item->unk2 & 0x10;
    return flags != 0;
}

s32 func_800ED5FC(Obj *obj) {
    void *held;
    Pos pos;
    Turn turn;
    s32 onFloor;
    Item *item;
    s32 active;
    s32 extra;

    held = func_800E8978(obj);
    func_800A6CC0(&pos, obj);
    onFloor = 0;
    if (func_800A4754(obj, obj, &obj->unk8)) {
        onFloor = func_800B4888(&pos) == 0;
    }
    if (func_800E1CC4(obj, 4)) {
        turn.dir = (obj->unk8 + 4) & 7;
        func_800A665C(obj, &turn);
    }
    if (held != 0) {
        func_800AE610(held, obj);
    } else {
        func_800E8FAC(obj);
    }
    if (onFloor) {
        item = func_800B4D80(&pos);
        active = item != 0 && item->unk0 == 0x10 && itemFlag10Set(item);
        if (active) {
            extra = func_800EC630(obj, item) ^ 1;
            item->unk2 &= 0xEF;
            func_80049CB4(0xD7, &pos);
            if (extra) {
                func_80049CB4(0x123, &pos);
            }
        }
    }
    return 1;
}
