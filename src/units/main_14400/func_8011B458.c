#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { char pad[0x18]; s16 offset18; char pad1A[2]; void (*func1C)(void *); } VTable;
typedef struct { char pad[0x1E]; u8 flags1E; char pad1F[5]; VTable *vtbl24; } Obj;
extern u16 D_8014767C;
extern s32 D_80147678;
s32 func_800A99D0(void);
s32 func_80049CB4(s32 id, ...);
void func_80049A04(u16 message_id, ...);
char *func_800A3B20(Obj *obj);
void func_800498E4(s32 message_id, ...);
void func_80049BF0(s32);
/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011B458(void *arg0, Obj *obj, void *item) {
    s32 busy = 0;
    if ((D_8014767C & 0xC) || func_800A99D0()) {
        busy = 1;
    }
    if (busy) {
        func_80049CB4(0x132);
        func_80049A04(0x225);
    } else {
        func_80049CB4(0x132);
        func_80049CB4(0x77, obj);
        if ((obj->flags1E >> 2) & 1) {
            D_80147678 = 2;
            func_800498E4(0xE1, func_800A3B20(obj));
            func_80049BF0(0);
        } else {
            obj->vtbl24->func1C((char *)obj + obj->vtbl24->offset18);
        }
    }
}
