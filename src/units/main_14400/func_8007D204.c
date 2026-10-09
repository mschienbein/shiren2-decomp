#include "common.h"

/* Status snapshot words, owned once by this joint unit: func_8007D204 and
 * func_8007D320 both need the local definitions (stores in jal/jr delay slots). */
s32 D_8013DE80 = 0;
s32 D_8013DE84 = 0;
s32 D_8013DE88 = 0;
s32 D_8013DE8C = 0;
s32 D_8013DE90 = 0;
s32 D_8013DE94 = 0;
extern s32 D_8013DE9C;

/* The caller stores every result without re-extending it: all six return int. */
s32 func_80041C44(void);
s32 func_80041BCC(void);
s32 func_80041BF4(void);
s32 func_80041C1C(void);
s32 func_80041C54(void);
s32 func_80041ACC(void);
extern s32 func_8007D388(void);
extern void func_80083568(void);

/* Snapshot six status values into the cached globals. */
void func_8007D204(void)
{
    D_8013DE80 = func_80041C44();
    D_8013DE84 = func_80041BCC();
    D_8013DE88 = func_80041BF4();
    D_8013DE8C = func_80041C1C();
    D_8013DE90 = func_80041C54();
    D_8013DE94 = func_80041ACC();
}

void func_8007D264(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 before = func_8007D388();

    D_8013DE80 = arg0;
    D_8013DE84 = arg1;
    D_8013DE88 = arg2;
    D_8013DE8C = arg3;
    D_8013DE90 = arg4;
    D_8013DE94 = arg5;
    D_8013DE9C = arg6;
    if (before != func_8007D388()) {
        func_80083568();
    }
}

void func_8007D320(s32 value) {
    D_8013DE90 = value;
}
