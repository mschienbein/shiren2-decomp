#include "common.h"
/* Complete 0x1E4-byte menu, ending before D_80142894. */
typedef struct { char pad[0x4C]; const s32 *field_4C; char pad50[0x180]; s32 field_1D0; const s32 *field_1D4; char pad1D8[0xC]; } Obj;
extern Obj D_801426B0;
extern const s32 D_80152EA8[], D_80151EC8[];
extern Obj *func_800953C0(Obj *);
void func_8009F244(void) { Obj *p = &D_801426B0; func_800953C0(p); p->field_4C = D_80152EA8; p->field_1D0 = -1; p->field_1D4 = D_80151EC8; }
