#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[6];
    u8 flags[4];
    u8 padA;
    u8 kind_0B;
} Record800CAAF0;

/* 0x20-byte reader object; only its vtable and the -1 handle are initialized inline. */
typedef struct {
    const void *vtable;
    u8 pad4[0xC];
    s32 x10;
    u8 pad14[0xC];
} Reader;

extern const u8 D_8015488C[8];
extern const unsigned char D_80151DF8[24];
extern u8 D_80138D30[];
extern u8 D_80138D40[];

void func_800CAFC0(Record800CAAF0 *record);
void func_800CABF8(unsigned char *record, u32 first, u32 second);
void func_800CADBC(Record800CAAF0 *self);
void func_80095064(void *, s32, void *);
void func_800950E4(Reader *reader, s32 arg);
void func_80095010(Reader *reader, s32 mode);

static inline void set_flag(u8 *flags, s32 bit)
{
    flags[bit >> 3] |= D_8015488C[bit & 7];
}

static inline s32 test_flag(u8 *flags, s32 bit)
{
    return flags[bit >> 3] & D_8015488C[bit & 7];
}

void func_800CAAF0(Record800CAAF0 *record)
{
    Reader reader;

    func_800CAFC0(record);
    set_flag(record->flags, 26);
    if (!test_flag(record->flags, 9)) {
        if (record->kind_0B == 0xB) {
            func_800CABF8((unsigned char *)record, 0xB, 0);
        } else {
            Reader *opened;

            reader.vtable = D_80151DF8;
            reader.x10 = -1;
            opened = &reader;
            if (record->kind_0B == 0xA) {
                func_80095064(opened, 0x258, D_80138D40);
            } else {
                func_80095064(opened, 0x257, D_80138D30);
            }
            func_800CABF8((unsigned char *)record, 0x14, 0);
            func_800950E4(&reader, -1);
            func_80095010(&reader, 2);
        }
    }
    func_800CADBC(record);
}
