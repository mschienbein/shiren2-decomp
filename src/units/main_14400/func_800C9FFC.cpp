#include "common.h"

/* g++ static-destruction function of the record TU (it sits right before
 * that TU's static-construction function func_800CA008, which constructs
 * D_80147680 through func_800CA760, and it is listed in the destructor table
 * at D_80148F24). The record class has an empty inline destructor: g++ keeps
 * the outgoing-argument frame of the integrated ~RecordObject(this, 2) call
 * and nothing else. */
struct RecordChild;
/* Whole 0x30-byte record object at D_80147680 (layout as in func_800CA008). */
struct RecordObject {
    unsigned char pad0[9];      /* +0x00..+0x08 */
    unsigned char flags9;       /* +0x09 */
    signed char valueA;         /* +0x0A */
    unsigned char padB[0x11];   /* +0x0B..+0x1B */
    RecordChild *child1C;       /* +0x1C */
    unsigned char value20;      /* +0x20 */
    unsigned char pad21[3];
    s32 value24;                /* +0x24 */
    s32 value28;                /* +0x28 */
    unsigned char pad2C[4];     /* +0x2C..+0x2F */

    ~RecordObject() {}
};

extern "C" {
extern RecordObject D_80147680;
void func_800C9FFC(void);
}

void func_800C9FFC(void)
{
    D_80147680.~RecordObject();
}
