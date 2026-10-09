#include "common.h"

typedef struct OSThread OSThread;
typedef struct OSMesgQueue OSMesgQueue;
extern void func_80027ED0(OSThread *, s32, void (*)(void *), void *, void *, s32);
extern void func_80032B50(OSThread *);
extern void func_8002F7C0(s32, OSMesgQueue *, void **, s32);
extern void *func_80026680(void);
extern void func_800265E0(void *, s32);
extern void func_80025DCC(void *, void *, s32);
extern void func_8006E590(void *);
extern void func_80025DB0(void *);
extern OSThread D_8003D518;
/* The idle thread has a 0x200-byte, eight-byte-aligned downward-growing stack. */
extern unsigned long long D_8003D6C8[0x200 / sizeof(unsigned long long)];
extern OSMesgQueue D_80039030;
extern void *D_80039048[200];
extern char D_801609B0[];
extern char D_14400[];
extern char D_001339F0[];
extern char D_136DC0[];
/* ODD_C: D_1339F0 names the main ROM end materialized at 0x80025D64-0x80025D68;
 * canonical D_001339F0 keeps its overlay ROM-start role, rematerialized at
 * 0x80025D74-0x80025D78. D_801E4EA0 names the main BSS end at 0x80025D44-
 * 0x80025D48; canonical func_801E4EA0 keeps the overlay RAM-start role,
 * rematerialized at 0x80025D7C-0x80025D80, as used by func_80060C54.
 * [INFERENCE] Distinct, coincident linker-boundary symbols explain this;
 * separate declarations prevent GCC from CSEing them into saved registers.
 * The new names reconstruct boundary roles, not recovered symbol spellings. */
extern char D_1339F0[], D_801E4EA0[];
extern s32 func_801E4EA0(void);
extern u32 func_800413C0(void);

void func_80025CC8(void *argument)
{
    /* Preserve the boot argument explicitly even though game entry ignores it. */
    func_80027ED0(&D_8003D518, 60, func_80025DB0, 0,
                 &D_8003D6C8[0x200 / sizeof(unsigned long long)], 127);
    func_80032B50(&D_8003D518);
    func_8002F7C0(150, &D_80039030, D_80039048, 200);
    func_80026680();
    /* local-arithmetic-qualification: linker-defined image boundaries are
     * addresses, not elements of a C array whose pointers can be subtracted. */
    func_800265E0(D_801609B0, (u32)D_801E4EA0 - (u32)D_801609B0);
    func_80025DCC(D_14400, (void *)func_800413C0, (u32)D_1339F0 - (u32)D_14400);
    func_80025DCC(D_001339F0, (void *)func_801E4EA0, (u32)D_136DC0 - (u32)D_001339F0);
    func_8006E590(argument);
}
