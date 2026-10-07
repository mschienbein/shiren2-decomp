#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos80107EC0;

typedef struct {
    Pos80107EC0 pos;
    u8 pad8[0x84];
    void *map8C;
} Obj80107EC0;

extern s32 func_80107E40(Obj80107EC0 *obj);
extern void *func_800B4D80(void *pos);
extern s32 func_800CD5C0(void *container, void *item);
extern void func_800AD868(Pos80107EC0 *pos);
extern u32 func_800B1C6C(void *pos);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(void *obj);
extern char *func_800AE674(void *obj);
extern void func_800497F0(s32 message_id, ...);

s32 func_80107EC0(Obj80107EC0 *obj) {
    Pos80107EC0 pos;
    Pos80107EC0 *p;
    void *tile;
    s32 name;
    char *actor;

    if (func_80107E40(obj) != 0) {
        p = &pos;
        p->x = obj->pos.x;
        p->y = obj->pos.y;
        tile = func_800B4D80(p);
        if (func_800CD5C0(obj->map8C, tile) == 0) {
            return 0;
        }
        func_800AD868(p);
        if (func_800B1C6C(p) & 0x2000) {
            name = func_80049CB4(0x26, obj);
        } else {
            name = func_80049CB4(0xDA, p);
        }
        actor = func_800A3B20(obj);
        func_800497F0(0x76, name, actor, func_800AE674(tile));
        return 1;
    }
    return 0;
}
