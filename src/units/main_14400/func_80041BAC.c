#include "common.h"
typedef struct { unsigned char pad_00[0x78]; s32 field_78; } State80041BAC;
extern State80041BAC *D_801476B8;
s32 func_80041BAC(void) {
    return D_801476B8->field_78;
}
