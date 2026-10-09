#include "common.h"

typedef unsigned char u8;

typedef struct { const void *vtable; u8 pad4[0xC]; s32 x10; u8 pad14[0xC]; } Reader;
typedef struct { u8 pad0[0x4C]; const void *vtable; u8 pad50[0x10]; Reader reader; } Loader;
extern const unsigned char D_80153110[144];
extern const unsigned char D_80151DF8[24];
extern const unsigned char D_80151E38[144];
Loader *func_800953C0(Loader *loader);
void func_8009FDD0(Loader *loader, s32 key);
s32 func_800957C0(Loader *loader, s32 *out, s32 a, void *b, s32 c);
void func_80095010(Reader *reader, s32 mode);
s32 func_80094E6C(s32 key) {
    Loader loader;
    s32 result;
    Loader *p = &loader;
    s32 failed;
    s32 value;
    Reader *reader;
    func_800953C0(p);
    p->vtable = D_80153110;
    loader.reader.vtable = D_80151DF8;
    loader.reader.x10 = -1;
    reader = &loader.reader;
    func_8009FDD0(p, key);
    failed = func_800957C0(p, &result, 1, 0, 0) != 1;
    if (failed) {
        func_80095010(reader, 2);
        p->vtable = D_80151E38;
        return 3;
    }
    value = result;
    func_80095010(reader, 2);
    p->vtable = D_80151E38;
    return value;
}
