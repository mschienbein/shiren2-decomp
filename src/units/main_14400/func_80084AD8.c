#include "common.h"

typedef unsigned short u16;
/* Partial view of the 0x74-byte task record returned by func_80084AB4. */
typedef struct Obj Obj;
typedef void (*Handler)(Obj *obj);
struct Obj {
    Handler handler;
    u16 pad4;
    u16 field_6;
    char pad8[8];
    u16 field_10;
};
Obj *func_80084AB4(s32 id);
void func_800843D8(Obj *obj);
void func_8008AF00(Obj *obj);

s32 func_80084AD8(s32 start, Handler handler) {
    s32 count = 0;
    s32 id = start;

    while (id >= 0) {
        Obj *obj = func_80084AB4(id);

        if (obj->handler == func_800843D8 || obj->handler == func_8008AF00) {
            id = obj->field_10;
        } else if (obj->handler == handler) {
            count++;
            break;
        }
        if (id == obj->field_6) {
            return count;
        }
    }
    return count;
}
