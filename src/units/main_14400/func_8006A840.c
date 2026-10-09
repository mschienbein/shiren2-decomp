#include "common.h"

typedef unsigned char u8;
typedef struct OSPiHandle OSPiHandle;

extern OSPiHandle *D_8016FD70; /* cartridge ROM PI handle */
extern u8 *D_8016FD6C;         /* heap mark */
extern u8 *D_8016FD64;         /* heap cursor */
extern s32 D_8016FD60;
extern u8 *D_8016FD68;         /* heap end */
extern s32 D_8013CA20;         /* heap ready flag */
extern u8 D_801F7200[];        /* heap start */

OSPiHandle *func_80026680(void); /* osCartRomInit */

void func_8006A840(void)
{
    D_8016FD70 = func_80026680();
    D_8016FD6C = D_801F7200;
    D_8016FD64 = D_801F7200;
    D_8016FD60 = 0;
    D_8016FD68 = (u8 *)0x8038F800;
    D_8013CA20 = 1;
}
