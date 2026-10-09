#include "common.h"
typedef struct { unsigned char pad_0[0x24]; unsigned char field_24; unsigned char field_25; unsigned char pad_26[2]; } Record;
extern s32 D_8016DB6C;
extern Record *D_801D40CC;
void func_80067380(void) { Record *record = D_801D40CC; Record *end = record + D_8016DB6C; for (; record < end; ++record) if (record->field_24) record->field_25 |= 1; }
