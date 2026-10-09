#include "common.h"
#include "pi_handle_view.h"
extern PiHandleView *D_80138B08;
extern PiResult func_80029FB0(PiHandleView *, PiWord, PiWord);
extern PiResult func_80029F50(PiHandleView *, PiWord, PiWord *);
void func_80043BB8(PiWord address, PiWord value) {
    PiWord observed;
    s32 retries;
    for (retries = 9; retries >= 0; retries--) {
        func_80029FB0(D_80138B08, address, value);
        func_80029F50(D_80138B08, address, &observed);
        if (value == observed) break;
    }
}
