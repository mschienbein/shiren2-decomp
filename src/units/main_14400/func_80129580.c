#include "common.h"

typedef unsigned char u8;

/*
 * Command handler of table D_801487D0: called with the interpreter state and the
 * byte cursor of the command stream; the result is the next cursor (unchanged here).
 * Partial state view: only the fields this handler writes are typed.
 */
typedef struct { u8 pad0[0x68]; s32 field_68; u8 pad6C[0xD5 - 0x6C]; u8 field_D5; } Obj80129580;

u8 *func_80129580(Obj80129580 *obj, u8 *cursor) {
    obj->field_D5 = 0;
    obj->field_68 = 0;
    return cursor;
}
