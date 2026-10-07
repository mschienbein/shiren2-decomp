#include "common.h"

/* libultra PI handle; only its address is carried here. */
typedef struct OSPiHandle OSPiHandle;

extern s32 D_8013CA20;
extern OSPiHandle *D_8016FD70; /* cartridge handle from func_80026680 (osCartRomInit) */

OSPiHandle *func_8006AC90(void)
{
    if (D_8013CA20 == 0) {
        return 0;
    }
    return D_8016FD70;
}
