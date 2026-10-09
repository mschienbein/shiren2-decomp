#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Embedded resource handle object; func_80048728 releases the handle at +0xC. */
typedef struct {
    u8 pad0[0xC];
    s32 handle_0C;
} Handle8009F430;

typedef struct {
    u8 pad0[0x74];
    Handle8009F430 handle_74;
} Obj8009F430;

extern void func_80048728(void *);

void func_8009F430(Obj8009F430 *obj)
{
    func_80048728(&obj->handle_74);
}
