#include "observed_fields_80058BA8.h"

/* Reads stay inside their guards and follow the original store order.
 * Output pointers may alias a source view or another output pointer.
 */
void func_80058BA8(u16 *out_3118, u16 *out_311e,
                   u8 *out_311a, u8 *out_311b)
{
    if (out_3118 != 0) {
        *out_3118 = D_80163118;
    }
    if (out_311e != 0) {
        *out_311e = D_8016311E;
    }
    if (out_311a != 0) {
        *out_311a = D_8016311A;
    }
    if (out_311b != 0) {
        *out_311b = D_8016311B;
    }
}
