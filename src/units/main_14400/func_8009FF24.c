#include "common.h"
/* Reader handle +0xC is closed through func_80048728; reader extent is 0x14. */
typedef struct { unsigned char pad0[0xC]; s32 handleC; } ReaderHandle;
typedef struct { void *vtable0; ReaderHandle handle4; } Reader;
typedef struct { unsigned char pad0[0x4C]; const void *vtable4C; unsigned char pad50[0x10]; Reader reader60; } Object;
extern const unsigned char D_80151E38[144];
extern void func_80095010(void *reader, s32 mode);
extern void func_800D8FA8(void *object);
void func_8009FF24(Object *object, s32 flags) {
    func_80095010(&object->reader60, 2);
    object->vtable4C = D_80151E38;
    if (flags & 1) func_800D8FA8(object);
}
