#include "common.h"
typedef struct { void *records; unsigned char *bits; s32 capacity; s32 used; } Pool;
/* Original 0x30-byte records and separate occupied-bit backing stores. */
extern Pool D_80143094, D_801430A4, D_801430B4, D_801430C4, D_801430D4, D_801430E4, D_801430F4;
extern unsigned char D_801C56A0[], D_801C7C20[], D_801C7C3C[], D_801C82CC[], D_801C82D4[], D_801C8E14[];
extern unsigned char D_801C8E1C[], D_801C8FFC[], D_801C9000[], D_801C9090[], D_801C9094[], D_801C9124[], D_801C9128[], D_801C91B8[];
extern Pool *func_800AF890(Pool *pool, void *records, unsigned char *bits, s32 capacity);
void func_800B06E0(void) {
    func_800AF890(&D_80143094, D_801C56A0, D_801C7C20, 200);
    func_800AF890(&D_801430A4, D_801C7C3C, D_801C82CC, 35);
    func_800AF890(&D_801430B4, D_801C82D4, D_801C8E14, 60);
    func_800AF890(&D_801430C4, D_801C8E1C, D_801C8FFC, 10);
    func_800AF890(&D_801430D4, D_801C9000, D_801C9090, 3);
    func_800AF890(&D_801430E4, D_801C9094, D_801C9124, 3);
    func_800AF890(&D_801430F4, D_801C9128, D_801C91B8, 3);
}
