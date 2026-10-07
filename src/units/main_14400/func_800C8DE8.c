#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 type; s32 payload[5]; } Message800C8DE8;
typedef struct { char pad[0x58]; s16 offset58; char pad5A[2]; s32 (*func5C)(void *, Message800C8DE8 *); } VTable;
typedef struct { char pad[0x24]; VTable *vtbl24; char pad28[0x2E]; u8 unk56; char pad57[0x1D]; u8 count74; } Obj;
extern s32 D_80147678;
extern Obj *D_801476B8;
void func_800E4EAC(Obj *);
u16 func_800E08B0(Obj *);
s32 func_800E2074(Obj *);
s32 func_80049CB4(s32, ...);
void func_800E1048(Obj *);
void func_800E4E90(Obj *);
s32 func_800A08D8(s32, s32, s32);
void func_800C8DE8(Obj *obj) {
    Message800C8DE8 args;
    s32 status;
    s32 result;
    args.type = 1;
    func_800E4EAC(obj);
    do {
        status = 0;
        if (obj->unk56 == 0) {
            return;
        }
        if (func_800E08B0(obj) == 0 || func_800E2074(obj) == 0) {
            status = 1;
        }
        if (status) {
            return;
        }
        func_80049CB4(0x8B, obj);
        obj->count74++;
        result = obj->vtbl24->func5C((char *)obj + obj->vtbl24->offset58, &args);
        func_800E1048(obj);
        if (result == 0) {
            return;
        }
        func_800E4E90(obj);
        func_80049CB4(0x132);
        func_800A08D8(1, -1, 2);
        func_80049CB4(0x89, D_801476B8);
        func_80049CB4(2);
        func_80049CB4(0xC, 0);
    } while (D_80147678 == 0);
}
