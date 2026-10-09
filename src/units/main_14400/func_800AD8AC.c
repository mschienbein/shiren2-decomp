#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { s32 kind; void *source; void *target; s32 fieldC; Position position; u32 field18, field1C; } Message;
typedef struct { u8 pad0[8]; short delta8, padA; void (*destroyC)(void *, s32); u8 pad10[8]; short delta18, pad1A; s32 (*query1C)(void *, s32); u8 pad20[0x18]; short delta38, pad3A; s32 (*dispatch3C)(void *, Message *); } VTable;
typedef struct { u8 type, field1, flags, field3; s32 field4; VTable *vtable; } Item;
extern s32 func_800AD714(Item *item, Position *position);
extern u32 func_800B1C6C(Position *position);
extern void func_800B1CEC(s32 mode, void *position);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800ADA00(void *item, void *position);
extern void func_800D3650(void *item);
static __inline__ s32 has_flag(Item *item, s32 mask) { return (item->flags & mask) != 0; }
s32 func_800AD8AC(Item *item, Position *position) {
    Message message;
    if (func_800AD714(item, position)) {
        s32 change = 0;
        s32 failed;
        if (func_800B1C6C(position) & 0x2000) change = item->field3 == 2;
        if (change) {
            func_800B1CEC(1, position);
            func_80049CB4(0x10B, position);
        }
        failed = func_800ADA00(item, position) != 1;
        if (!failed) {
            s32 mark = 0, result;
            if (item->vtable->query1C((char *)item + item->vtable->delta18, 0x22) && !(func_800B1C6C(position) & 0x2000)) mark = !has_flag(item, 0x40);
            if (mark) item->flags |= 0x20;
            message.kind = 0x1A;
            message.position = *position;
            result = item->vtable->dispatch3C((char *)item + item->vtable->delta38, &message);
            item->flags &= ~0x40;
            return result;
        }
        return 0;
    }
    func_800D3650(item);
    if (item != 0) item->vtable->destroyC((char *)item + item->vtable->delta8, 3);
    return 0;
}
