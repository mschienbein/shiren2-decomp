#include "common.h"
#include "item_records.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef struct Position { s32 x, y; } Position;
typedef Position Value;
typedef struct Message {
    s32 kind;
    u8 pad_04[0xC];
    Position position_10;
} Message;
typedef Message Msg;
typedef struct UnitVTable {
    u8 pad_00[8];
    short this_delta_08, slot_0A;
    void (*destroy_0C)(void *, s32);
} UnitVTable;
typedef struct Unit { u8 pad_00[0x24]; UnitVTable *vtable_24; } Unit;

extern void *func_800A8694(u8 kind, u8 variant, void *memory);
extern void func_800B4E7C(Position *position);
extern s32 func_800A5D2C(void *object, Value *position, s32 flags);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A58FC(void *object, Position *position);
extern void func_800498E4(s32 id, ...);
extern s32 func_800AF28C(Item *object, Msg *message);

s32 func_80128D0C(Item *object, Message *message)
{
    char *name = func_800AE674(object);
    if (message->kind == 0x1A) {
        s32 allowed = 0;
        if (!(object->flags02 & 0x40))
            allowed = D_80142F18.mode != 0x77;
        if (allowed) {
            Unit *unit = func_800A8694(0x29, object->payload.appearance.flags0D & 0x7F, 0);
            if (unit) {
                Position position;
                Position *destination = &position;
                destination->x = message->position_10.x;
                destination->y = message->position_10.y;
                func_800B4E7C(destination);
                if (func_800A5D2C(unit, destination, 1)) {
                    func_80049CB4(0x120, destination);
                    func_800A58FC(unit, destination);
                } else {
                    func_800498E4(0x87, name);
                    unit->vtable_24->destroy_0C((u8 *)unit + unit->vtable_24->this_delta_08, 3);
                }
            }
        }
        return 1;
    }
    return func_800AF28C(object, message);
}
