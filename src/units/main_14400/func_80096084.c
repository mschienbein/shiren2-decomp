#include "common.h"

/* Base widget slot +0x34 (confirm): s32 (void *self, s32 *out). The only caller,
 * func_80046CB0 (0x80046D94), passes the receiver and its output word; the base
 * class accepts every confirmation and reads neither argument. */
s32 func_80096084(void *self, s32 *out)
{
    return 1;
}
