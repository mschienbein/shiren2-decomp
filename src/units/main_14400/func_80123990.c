#include "common.h"
#include "item_records.h"
extern ItemMethods D_8015FC60;
extern void *func_8010E020(void *object, s32 kind);
extern unsigned short func_800AB044(void);
Item *func_80123990(Item *object) {
    func_8010E020(object, 0xCC);
    object->field08 = &D_8015FC60;
    /* func_8010E020 constructs the kind-0xE base in this amount object. */
    object->payload.amount.signed0C = func_800AB044();
    return object;
}
