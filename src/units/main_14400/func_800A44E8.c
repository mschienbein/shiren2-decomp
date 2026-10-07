#include "common.h"

typedef unsigned char u8;
typedef struct Obj800A44E8 Obj800A44E8;
typedef struct Entity800A44E8 Entity800A44E8;

/* Slot target: the receiver and target entity are passed by the slot contract
 * (see scratch/omp/b4-rfix2/s15/report.json) but this implementation only
 * clears the output byte and reports 0. */
s32 func_800A44E8(Obj800A44E8 *obj, Entity800A44E8 *target, u8 *out)
{
    *out = 0;
    return 0;
}
