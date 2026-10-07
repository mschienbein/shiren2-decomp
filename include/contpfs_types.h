#ifndef SDK_A_CONTPFS_PRIVATE_TYPES_H
#define SDK_A_CONTPFS_PRIVATE_TYPES_H

#include "controller_ram.h"
typedef unsigned long long u64;

/* Observed field prefix only. No allocation or complete historical type claim. */
typedef struct {
    s32 field00;
    void *field04;
    s32 field08;
    u8 field0c[32];
    u8 field2c[32];
    s32 field4c;
    s32 field50;
    s32 field54;
    s32 field58;
    s32 field5c;
    s32 field60;
    u8 field64;
    u8 field65;
} ResidentPfs;

/* Original loads/stores establish a 32-byte record with paired words at8/16. */
typedef struct {
    u32 field00;
    u32 field04;
    u64 field08;
    u64 field10;
    u16 field18;
    u8 field1a;
    u8 field1b;
    u16 field1c;
    u16 field1e;
} ResidentPackId;

typedef union {
    u16 value;
    struct { u8 field00; u8 field01; } bytes;
} ResidentInodeUnit;

typedef struct { ResidentInodeUnit units[128]; } ResidentInode;

extern s32 D_80036F60;
extern u8 D_80036F64;
extern ResidentInode D_800411D0;

extern u32 func_8002A9B0(void);
extern s32 func_8002F640(ResidentPfs *, u8);
extern s32 func_800261B0(const void *, const void *, s32);

u16 func_80026800(u8 *, s32);
s32 func_80026834(u16 *, u16 *, u16 *);
s32 func_80026878(ResidentPfs *, ResidentPackId *, ResidentPackId *);
s32 func_80026B64(ResidentPfs *, ResidentPackId *);
s32 func_80026CC8(ResidentPfs *);
s32 func_80026E94(ResidentPfs *);
s32 func_80026F4C(ResidentPfs *, ResidentInode *, u8, u8);

typedef char sdk_a_check_int[(sizeof(s32) == 4) ? 1 : -1];
typedef char sdk_a_check_pointer[(sizeof(void *) == 4) ? 1 : -1];
typedef char sdk_a_check_id[(sizeof(ResidentPackId) == 32) ? 1 : -1];
typedef char sdk_a_check_inode[(sizeof(ResidentInode) == 256) ? 1 : -1];

#endif
