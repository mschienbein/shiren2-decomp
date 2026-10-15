#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { ItemMessage base; void *source; void *target; s32 fieldC; Position position; u32 field18, field1C; } Message;
extern s32 func_800AD714(Item *item, Position *position);
extern u32 func_800B1C6C(Position *position);
extern void func_800B1CEC(s32 mode, void *position);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800ADA00(Item *item, void *position);
extern void func_800D3650(void *item);
static __inline__ s32 has_flag(Item *item, s32 mask) { return (item->flags02 & mask) != 0; }
s32 func_800AD8AC(Item *item, Position *position) {
    Message message;
    if (func_800AD714(item, position)) {
        s32 change = 0;
        s32 failed;
        if (func_800B1C6C(position) & 0x2000) change = item->field03 == 2;
        if (change) {
            func_800B1CEC(1, position);
            func_80049CB4(0x10B, position);
        }
        failed = func_800ADA00(item, position) != 1;
        if (!failed) {
            s32 mark = 0, result;
            if (item->field08->test1C((char *)item + item->field08->delta18, 0x22) && !(func_800B1C6C(position) & 0x2000)) mark = !has_flag(item, 0x40);
            if (mark) item->flags02 |= 0x20;
            message.base.kind = 0x1A;
            message.position = *position;
            result = item->field08->handle3C((char *)item + item->field08->delta38, &message.base);
            item->flags02 &= ~0x40;
            return result;
        }
        return 0;
    }
    func_800D3650(item);
    if (item != 0) item->field08->method0C((char *)item + item->field08->delta08, 3);
    return 0;
}
