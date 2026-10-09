#include "common.h"

typedef unsigned char u8;
typedef struct { u8 b[4]; } Dims8009D910;
typedef struct { s32 w[4]; } Desc8009D910;
typedef struct {
    u8 pad00[0x54]; s32 mode54; u8 values58[0xA2]; u8 padFA[2];
    s32 countFC; u8 pad100[0x20]; unsigned short text120;
    u8 pad122[6]; s32 field128;
} Loader;
extern Desc8009D910 D_8014AB88;
extern s32 func_800D81D4(s32 id);
extern s32 func_800D8040(s32 id);
extern void func_8009D910(Loader *obj, Desc8009D910 *src, Dims8009D910 *dims);


void func_8009F680(Loader *loader, s32 arg)
{
    Dims8009D910 dims;
    loader->mode54 = arg;
    loader->countFC = 0;
    loader->field128 = 0;
    dims.b[0] = 10;
    dims.b[1] = 1;
    if (loader->mode54) {
        s32 i;
        for (i = 0; i < 0xA2; i++) {
            /* Both byte offsets remain inside the containing Loader object. */
            u8 *address = (u8 *)loader + i;
            address[0x58] = func_800D81D4(i) >= 0;
        }
        dims.b[3] = 0xA2;
        loader->text120 = 0x291;
    } else {
        s32 i;
        for (i = 0; i < 0xA2; i++) {
            if (func_800D8040(i)) loader->values58[loader->countFC++] = i;
        }
        dims.b[3] = loader->countFC;
        loader->text120 = 0x292;
    }
    func_8009D910(loader, &D_8014AB88, &dims);
}
