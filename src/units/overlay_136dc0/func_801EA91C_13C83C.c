#include "func_801EA91C_13C83C/types.h"
extern void *func_800AC244(u8 id);
extern s32 func_801EA990_13C8B0(s32 owner, OvlItem *item, s32 value);

/* ROM 0x140340 loads a full word; VRAM 0x801EA928 (ROM 0x13C848)
 * converts the ID to a byte inside this provider, not in its caller. */
void func_801EA91C_13C83C(s32 owner, s32 id, s32 value)
{
    u8 byte_id = id;
    OvlItem *item = func_800AC244(byte_id);
    if (item) {
        s32 failed = func_801EA990_13C8B0(owner, item, value) ^ 1;
        if (failed) {
            item->field08->method0C((u8 *)item + item->field08->delta08, 3);
        }
    }
}
