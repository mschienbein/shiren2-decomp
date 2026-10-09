#include "common.h"

/* Embedded 0x10-byte buffer; func_80048728 reads and clears its handle at +0xC. */
typedef struct { unsigned char pad00[0xC]; s32 handle0C; } Buffer;
typedef struct {
    s32 field00;
    Buffer field04;
} Obj80095190;

extern void func_80048728(void *);

void func_80095190(Obj80095190 *obj)
{
    func_80048728(&obj->field04);
}
