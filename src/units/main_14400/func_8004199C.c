#include "common.h"

extern void *D_801476B8;
/* The original wrapper explicitly passes the manager as the method receiver. */
extern s32 func_800EC69C(void *self);

s32 func_8004199C(void)
{
    return (unsigned char)func_800EC69C(D_801476B8);
}
