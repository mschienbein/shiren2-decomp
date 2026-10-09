#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x72]; u8 field_72; } Obj;
typedef struct { u8 pad_00[8]; short delta_08; short index_0A; void (*method_0C)(void *, s32); } ItemMethods;
typedef struct { u8 pad_00[8]; ItemMethods *field_08; } Item;
typedef struct { s32 field_00, field_04; } Entry;
typedef struct { s32 field_00; u8 pad_04[12]; Entry *field_10; } Message;
extern u8 D_801531A0[];
extern s32 func_800E1CC4(Obj *obj, s32 kind);
extern s32 func_800E1CD4(Obj *obj, s32 kind);
extern s32 func_80049CB4(s32 id, ...);
extern Item *func_800F1568(Obj *obj, s32 force);
extern s32 func_800AD714(Item *item, Obj *dest);
extern s32 func_800AD8AC(Item *item, Obj *dest);
extern s32 func_800F27A4(Obj *obj, Message *message);
s32 func_80101CEC(Obj *obj, Message *message)
{
    if (message->field_00 == 10) {
        s32 blocked = 0;
        Item *item;
        if ((D_801531A0[message->field_10->field_04] & 0x20) ||
            func_800E1CC4(obj, 2) || func_800E1CD4(obj, 15)) {
            blocked = 1;
        }
        if (!blocked) {
            func_80049CB4(0x5B, obj);
            func_80049CB4(0x87, obj);
            item = func_800F1568(obj, 0);
            if (item != 0) {
                if (func_800AD714(item, obj)) {
                    func_800AD8AC(item, obj);
                } else {
                    item->field_08->method_0C((u8 *)item + item->field_08->delta_08, 3);
                }
            }
        }
        obj->field_72 &= 0xF7;
    }
    return func_800F27A4(obj, message);
}
