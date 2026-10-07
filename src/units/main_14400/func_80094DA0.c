#include "common.h"

/* Partial word views; historical field types and result API are unproved. */
void func_80094DA0(void *arg0)
{
    unsigned char *bytes = arg0;

    *(u32 *)(bytes + 0x84) = 0;
    *(u32 *)(bytes + 0x88) = 0;
}
