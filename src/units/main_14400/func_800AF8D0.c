#include "common.h"
/* Same fixed 0x30-byte pool records used by func_800B0164. */
typedef struct { s32 words[0x30 / 4]; } PoolRecord;
typedef struct { PoolRecord *field_0; unsigned char *field_4; s32 field_8; } Object;
extern PoolRecord *D_80143104;
void func_800AF8D0(Object *arg) { unsigned char *p=arg->field_4; s32 count=(arg->field_8+7)/8; while(count-- > 0) *p++=0; D_80143104=0; }
