#include "common.h"
typedef unsigned char u8;
typedef struct { void *vtable; u8 pad04[0xC]; s32 handle_10; u8 pad14[0xC]; } Reader;
typedef struct { u8 pad00[0x4C]; const void *vtable_4C; u8 pad50[0xC]; Reader reader_5C; } Object;
extern const unsigned char D_80151E38[144];
extern void func_80095010(Reader *, s32);
extern void func_800D8FA8(void *);
void func_8009AD74(Object *object, s32 flags)
{
    func_80095010(&object->reader_5C, 2);
    object->vtable_4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(object);
    }
}
