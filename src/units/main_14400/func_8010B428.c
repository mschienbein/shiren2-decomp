#include "common.h"

typedef struct {
    void *table_00;
    void *vtable_04;
    unsigned char *items_08;
    unsigned char capacity_0C, start_0D, count_0E;
} List;
typedef struct { unsigned char pad_00[0xCC]; List list_CC; } Object;
extern void func_800EE97C(Object *object, void *target);
extern void func_800CE918(List *list, void *target);

void func_8010B428(Object *object, void *target)
{
    func_800EE97C(object, target);
    func_800CE918(&object->list_CC, target);
}
