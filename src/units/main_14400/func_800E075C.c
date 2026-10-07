#include "common.h"

/* Partial field view; the original owner and complete extent are unknown. */
typedef struct {
    unsigned char prefix[0x28];
    unsigned short field_28;
    unsigned short field_2A;
} Fields_800E075C;

/* Side-effect view: the historical return prototype remains unresolved. */
void func_800E075C(Fields_800E075C *arg0)
{
    arg0->field_28 = arg0->field_2A;
}
