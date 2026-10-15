#include "common.h"
#include "item_records.h"

extern char D_8015FC54[];
char *func_800AC990(void *obj);
s32 func_8005EF08(char *dst, const char *fmt, ...);

char *func_801239FC(Item *obj, char *dest) {
    /* D_8015FC60 is installed by the kind-0xE constructor func_80123990. */
    s32 value = obj->payload.amount.signed0C;

    func_8005EF08(dest, D_8015FC54, value, func_800AC990(obj));
    return dest;
}
