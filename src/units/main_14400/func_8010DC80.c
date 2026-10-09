#include "common.h"

typedef unsigned short u16;
typedef struct S S;
typedef struct { s32 field00; void *source04; void *target08; } Args;
extern s32 func_8010CD1C(S *s);
extern void func_800A7B18(void *target, void *source, s32 amount, s32 kind);

void func_8010DC80(S *self, Args *args) {
    void *target = args->target08;
    void *source = args->source04;
    func_800A7B18(target, source, (u16)func_8010CD1C(self), 6);
}
