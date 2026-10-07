#include "common.h"
#include "pi_handle_view.h"

extern PiHandleView *D_80138B08;
extern PiResult func_80029F50(PiHandleView *handle, PiWord devAddr, PiWord *data);

/* Reads one PI bus word until two consecutive reads agree (at most ten retries). */
u32 func_80043B48(u32 deviceAddress) {
    PiWord old;
    PiWord next;
    s32 i;

    func_80029F50(D_80138B08, deviceAddress, &old);
    for (i = 9; i >= 0; i--) {
        func_80029F50(D_80138B08, deviceAddress, &next);
        if (old == next) {
            break;
        }
        old = next;
    }
    return (u32)old;
}
