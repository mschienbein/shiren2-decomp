#include "common.h"

typedef struct { unsigned char pad00[0xC]; s32 handle0C; } Buffer;
typedef struct { s32 field00; Buffer field04; } Obj;
extern void func_80048728(void *);

void func_80095384(Obj *obj)
{
    func_80048728(&obj->field04);
}
