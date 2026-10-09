#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0xD]; u8 unkD[7]; s32 unk14; } Obj;
void func_800CB154(u32 arg0, char *formatDst);
char *func_80083C90(char *dst, char *src);
void func_800CAD04(Obj *obj, char *arg1, char *dst) {
    func_800CB154((u32)obj->unk14, arg1);
    func_80083C90(dst, (char *)obj->unkD);
}
