#include "common.h"

typedef struct {
    unsigned char data[11];
} Entry11;

extern unsigned char D_80142F24;
extern Entry11 D_80156BA0[];

Entry11 *func_800AA900(void) {
    return &D_80156BA0[D_80142F24];
}
