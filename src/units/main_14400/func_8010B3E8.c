#include "common.h"

/* The embedded list at 0xCC includes its vtable at +4 and byte-count fields at +0xC. */
typedef struct {
    void *table_00;
    void *vtable_04;
    unsigned char *items_08;
    unsigned char capacity_0C, start_0D, count_0E;
} List;
typedef struct Obj8010B3E8 {
    unsigned char pad_00[0xCC];
    List list_CC;
} Obj8010B3E8;

void func_800CE3E8(void *sub, void *list);

void func_8010B3E8(Obj8010B3E8 *obj, void *sub)
{
    func_800CE3E8(sub, &obj->list_CC);
}
