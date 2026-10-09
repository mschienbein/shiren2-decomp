#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[0x50]; u16 id_50; u16 pad_52; char *text_54[8]; s32 count_74; } Obj;
extern char *func_80048480(u16 id);
extern char *func_80083D04(char *dst, char *src);
void func_8009C2C8(Obj *obj, s32 index, char *dst) {
    *dst = 0;
    if (obj->id_50 != 0) {
        if (index == 0) func_80083D04(dst, func_80048480(obj->id_50));
    } else if (index < obj->count_74) {
        func_80083D04(dst, obj->text_54[index]);
    }
}
