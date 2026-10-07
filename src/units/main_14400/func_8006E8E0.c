#include "common.h"

/* Partial access view; the original allocation and API remain unproved. */
void func_8006E8E0(void *arg0)
{
    unsigned char *bytes = arg0;

    bytes[0x00] = 0;
    *(u32 *)(bytes + 0x08) = 0;
    *(u32 *)(bytes + 0x0C) = 0;
    *(u32 *)(bytes + 0x14) = 0;
    *(u32 *)(bytes + 0x18) = 0;
    *(u32 *)(bytes + 0x24) = 0;
    *(u32 *)(bytes + 0x04) = 0;
    *(u32 *)(bytes + 0x10) = 0;
    *(u32 *)(bytes + 0x20) = 0;
}
