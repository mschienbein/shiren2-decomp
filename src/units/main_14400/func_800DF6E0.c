#include "common.h"

typedef struct {
    unsigned short field00;
    unsigned short pad02;
    void *field04;
} Record_800DF6E0;

extern unsigned char D_80157FA8[];
extern unsigned char D_80158A78[];

Record_800DF6E0 *func_800DF6E0(Record_800DF6E0 *record)
{
    record->field04 = D_80157FA8;
    record->field00 = 0;
    record->field04 = D_80158A78;
    return record;
}
