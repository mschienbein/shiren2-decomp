#include "common.h"
#include "item_records.h"

Item *func_800D55F0(Handle *handle, u32 index) {
    if (index < 3) {
        return func_800D4A60(handle, index);
    }
    return 0;
}
