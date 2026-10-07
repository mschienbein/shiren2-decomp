#include "common.h"

typedef struct {
    unsigned char pad00[0x74];
    unsigned char field74;
} Record_80116928;

extern unsigned char D_801486DC;
extern u32 D_801486E0;
extern unsigned char D_801486E4;
extern Record_80116928 *D_801476B8;

s32 func_80116928(unsigned char code)
{
    if (D_801486E4 == 0) {
        D_801486DC = 0;
        D_801486E0 = 0;
        D_801486E4 = 0;
        D_801476B8->field74++;
    }
    if (D_801486E0 == 0 && D_801486E4 < 20) {
        D_801486E4++;
        if (code != 0xE8) {
            D_801486DC++;
        }
        return 1;
    }
    return 0;
}
