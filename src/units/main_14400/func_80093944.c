#include "common.h"
/* The entry layout belongs to the table helpers; this context only forwards it. */
typedef struct TableEntry TableEntry;
typedef struct { s32 field_0; TableEntry *field_4; } Context;
typedef struct { void *container; void *item; } Pair;
extern void *func_800D8FB0(u32 size);
extern unsigned char *func_800DDDE0(void *),*func_800DE130(void *),*func_800DE460(void *,TableEntry *),*func_800DEBC0(void *);
extern s32 func_8009A038(void *);
extern Pair func_8009A054(void *,s32);
extern void func_800D05A4(void *,Pair *);
/* The original call contract retains this first receiver, intentionally unused. */
void *func_80093944(void *unused,s32 type,Context *ctx,void *source) {
    unsigned char *result;
    void *storage;
    s32 count,i;
    switch(type) {
    case 0x28:
        storage = func_800D8FB0(0xC4);
        result = func_800DDDE0(storage);
        break;
    case 0x29:
        storage = func_800D8FB0(0xC4);
        result = func_800DE130(storage);
        break;
    case 0x2A:
        storage = func_800D8FB0(0xC8);
        result = func_800DE460(storage, ctx->field_4);
        break;
    case 0x2C:
        storage = func_800D8FB0(0xC4);
        result = func_800DEBC0(storage);
        break;
    default: return 0;
    }
    count=func_8009A038(source);
    i=0;
    for(;;) {
        Pair item;
        if(!(i<count)) break;
        item = func_8009A054(source,i);
        func_800D05A4(result+0xB0,&item);
        ++i;
    }
    return result;
}
