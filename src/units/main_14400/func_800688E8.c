#include "common.h"
typedef struct { unsigned char unk0; char pad1[0x23]; } Entry24;
extern unsigned char D_8016DC11;
extern Entry24 D_8013C4FC[];
extern s32 D_801E4E74;
void func_800688E8(void){ D_801E4E74 = D_8013C4FC[D_8016DC11].unk0; }
