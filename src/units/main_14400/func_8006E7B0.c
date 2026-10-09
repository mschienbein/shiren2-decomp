#include "common.h"

typedef struct {
    s32 m[4][4];
} Mtx_8006E7B0;

s32 D_8013D454 = 0;
extern Mtx_8006E7B0 D_801A7100;
extern void func_8002CDC0(Mtx_8006E7B0 *mtx);
extern void func_80071040(void);

void func_8006E7B0(void) {
    D_8013D454 = 0;
    func_8002CDC0(&D_801A7100);
    func_80071040();
}
