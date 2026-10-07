#include "common.h"
#include "byte_move_view.h"

void func_800CE87C(ByteMoveView *record, s32 source, s32 destination)
{
    unsigned char saved;
    s32 toward_lower = (u32)destination < (u32)source;

    if (source != destination) {
        saved = record->bytes[source];
        if (toward_lower) {
            do {
                record->bytes[source] = record->bytes[(s32)((u32)source - 1U)];
                source = (s32)((u32)source - 1U);
            } while ((u32)destination < (u32)source);
        } else {
            while ((u32)source < (u32)destination) {
                record->bytes[source] = record->bytes[(s32)((u32)source + 1U)];
                source = (s32)((u32)source + 1U);
            }
        }
        record->bytes[destination] = saved;
    }
}
