#include "common.h"
typedef struct { unsigned char bytes[4]; } MessagePayload;
typedef struct { char pad[0xC]; void *unkC; MessagePayload unk10; } Obj;
void func_800498E4(s32 message_id, ...);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);
s32 func_800ACB8C(void *, MessagePayload *);
s32 func_800DD04C(Obj *o){
    s32 r = func_800ACB8C(o->unkC, &o->unk10) ^ 1;
    if (r) { func_800498E4(0x297); func_800A08D8(1, -1, 0); }
    return 1;
}
