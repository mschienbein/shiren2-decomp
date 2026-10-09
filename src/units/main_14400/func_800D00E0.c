#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x28]; short adjust_28; short pad2A; void (*read_2C)(void *, s32, void *); } Methods;
typedef struct { u8 pad0[0x18]; Methods *field_18; } Obj;
typedef Obj Target;
typedef struct Item Item;
typedef struct Table Table;
typedef struct { u8 pad0[0x18]; Item *field_18; } Dst;
extern const char D_80154544[];
extern Table D_80143094;
extern void func_800CA4E8(Obj *, void *);
extern void func_800CE9C4(void *, Target *);
extern Item *func_800AFD78(Table *table, u8 index);
void func_800D00E0(unsigned char *arg0, Obj *obj) {
    u8 id;
    func_800CA4E8(obj, (void *)D_80154544);
    func_800CE9C4(arg0, obj);
    obj->field_18->read_2C((u8 *)obj + obj->field_18->adjust_28, 1, &id);
    ((Dst *)arg0)->field_18 = func_800AFD78(&D_80143094, id);
}
