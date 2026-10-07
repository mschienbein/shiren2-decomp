#include "common.h"

typedef unsigned char u8;

/*
 * Item-container getter (actor vtable slot +0x98/+0x9C, called by func_800E8A68;
 * the populated counterpart is func_800EE07C). Actors without a container return
 * null. The receiver is part of the slot contract and is intentionally unused.
 */
u8 *func_800EB0B8(u8 *self)
{
    return 0;
}
