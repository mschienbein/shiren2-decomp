#include "field_views_8006A0AC.h"

/* Both recovered callers consume output bytes, with no observed scalar-result
 * use. The historical return declaration and complete caller set are unknown. */
void func_8006A0AC(unsigned char *arg0, unsigned char *arg1,
                   unsigned char *arg2, unsigned char *arg3)
{
    if (arg0 != 0) {
        *arg0 = D_8013CA00;
    }
    if (arg1 != 0) {
        *arg1 = D_8013CA01;
    }
    if (arg2 != 0) {
        *arg2 = D_8013CA02;
    }
    if (arg3 != 0) {
        *arg3 = D_8013CA03;
    }
}
