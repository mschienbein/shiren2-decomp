#include "common.h"
typedef struct { unsigned char field_00[0x40]; short field_40; unsigned char (*field_44)(void *, s32); } VTable;
typedef struct { unsigned char field_00[8]; VTable *field_08; } Object;
extern char D_80147620[];
extern s32 func_800C587C(void *, unsigned char);
extern unsigned char func_800C57CC(void *, unsigned char);
s32 func_8010FF20(Object *object) {
    unsigned char choices[2]; s32 count = 0; s32 sum;
    sum = object->field_08->field_44((char *)object + object->field_08->field_40, 0x46);
    sum += object->field_08->field_44((char *)object + object->field_08->field_40, 0xA1);
    if (sum > 100) sum = 100;
    if (func_800C587C(D_80147620, sum)) choices[count++] = 0xA1;
    if (func_800C587C(D_80147620, object->field_08->field_44((char *)object + object->field_08->field_40, 0x45))) choices[count++] = 0xCC;
    if (!count) return 0;
    { s32 index = count == 1 ? 0 : func_800C57CC(D_80147620, count - 1); return choices[index]; }
}
