#include "common.h"
typedef unsigned short u16;

typedef struct {
    unsigned char pad0[0xD];
    unsigned char code[5];
    unsigned char pad12[2];
    s32 unk14;
    s32 unk18;
} Obj;

extern unsigned char D_80147600[];
extern unsigned char func_800C57A0(void *rng);
extern void func_800C5CF8(void *rng, s32 seed);
extern u32 func_800C598C(void *rng);
extern char *func_80048480(u16 id);
extern void func_800CADBC(Obj *obj);

void func_800CAC04(Obj *obj) {
    s32 i;
    s32 shift;
    s32 count;
    u32 seed;
    u32 bits;
    u32 low;
    u32 index;
    u32 key;
    char *table;
    unsigned char *code;

    count = 99;
    if (obj->unk14 == 0) {
        seed = obj->unk18;
        obj->unk14 = seed;
        key = func_800C57A0(D_80147600);
        func_800C5CF8(D_80147600, seed ^ (key << 24));
        do {
            func_800C598C(D_80147600);
            count--;
        } while (count != -1);
        low = func_800C598C(D_80147600) & 0xFFF;
        bits = (key << 12) | low;
        table = func_80048480(0x4DC);
        code = obj->code;
        i = 0;
        shift = 15;
        while (1) {
            if (i >= 4) {
                break;
            }
            index = (bits >> shift) & 0x1F;
            code[i] = table[index];
            shift -= 5;
            i++;
        }
        code[4] = 0;
        func_800CADBC(obj);
    }
}
