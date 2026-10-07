#include "common.h"

/* Observed signed word access view; historical record ownership/type is unknown.
 * Both arguments may overlap. Later fields are read after earlier stores. */
void func_800A3448(s32 *arg0, s32 *arg1)
{
    if (arg0[1] < arg1[1]) {
        arg0[1] = arg1[1];
    }
    if (arg0[0] < arg1[0]) {
        arg0[0] = arg1[0];
    }
    if (arg0[3] > arg1[3]) {
        arg0[3] = arg1[3];
    }
    if (arg0[2] > arg1[2]) {
        arg0[2] = arg1[2];
    }
}
