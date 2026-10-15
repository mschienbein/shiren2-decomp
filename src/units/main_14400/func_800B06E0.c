#include "common.h"
#include "item_records.h"
/* Record storage (0x30 * capacity bytes each) and separate occupancy-bit stores
 * ((capacity + 7) / 8 bytes each), laid out back to back from D_801C56A0. */
extern Item D_801C56A0[200], D_801C7C3C[35], D_801C82D4[60], D_801C8E1C[10];
extern Item D_801C9000[3], D_801C9094[3], D_801C9128[3];
extern unsigned char D_801C7C20[], D_801C82CC[], D_801C8E14[], D_801C8FFC[], D_801C9090[], D_801C9124[], D_801C91B8[];
void func_800B06E0(void) {
    func_800AF890(&D_80143094, D_801C56A0, D_801C7C20, 200);
    func_800AF890(&D_801430A4, D_801C7C3C, D_801C82CC, 35);
    func_800AF890(&D_801430B4, D_801C82D4, D_801C8E14, 60);
    func_800AF890(&D_801430C4, D_801C8E1C, D_801C8FFC, 10);
    func_800AF890(&D_801430D4, D_801C9000, D_801C9090, 3);
    func_800AF890(&D_801430E4, D_801C9094, D_801C9124, 3);
    func_800AF890(&D_801430F4, D_801C9128, D_801C91B8, 3);
}
