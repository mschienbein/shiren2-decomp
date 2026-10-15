#include "common.h"

/* Sound-request constructor (callers: func_800459A8, func_80045E50).
   Scoped exception: scratch/omp/rulings/ORIGINAL-BUG-DANGLING-PARAM.md. */

typedef struct { signed char x; signed char y; } Pair800459A8;
typedef struct { s32 a, b; } Key800453E4;
typedef struct {
    /* ORIGINAL BUG: pos is left pointing at func_80045370's by-value pos
       parameter, which the ROM homes in the caller's o32 a2 argument slot
       (caller sp+8; 0x80045374 sh a2,8(sp)).  That parameter's lifetime ends
       when func_80045370 returns, but the callers read it later through this
       field: func_800459A8 (call 0x800459E4, then func_800453B8 at 0x800459F0)
       and func_80045E50 (call 0x80045EDC, then func_800453B8 at 0x80045F0C or
       func_80045468 at 0x80045F20); func_800453B8 -> func_800453E4 ->
       func_8004552C reads pos[1], func_8004559C and func_80045468 read pos[0].
       It works only because nothing stores to the caller's sp+8..9 between
       the call and those reads. */
    Pair800459A8 *pos;
    unsigned char kind04;
    unsigned char pad05[3];
    Key800453E4 field08;
    Key800453E4 field10;
} Buf800459A8;
extern Key800453E4 *D_801476B8;

Buf800459A8 *func_80045370(Buf800459A8 *buf, Key800453E4 *value, Pair800459A8 pos)
{
    buf->field08 = *value;
    buf->field10 = *D_801476B8;
    /* ORIGINAL BUG: stores the address of the by-value parameter pos
       (0x800453A4 addiu v1,sp,8; 0x800453A8 sw v1,0(v0)); it dangles once this
       function returns, and the callers above dereference it afterwards. */
    buf->pos = &pos;
    buf->kind04 = 5;
    return buf;
}
