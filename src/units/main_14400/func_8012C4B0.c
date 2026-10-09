#include "common.h"

typedef unsigned char u8;

/* Sound bank header: seven file-relative pointers at 0x0C..0x24 relocated on first use. */
typedef struct Bank8012C4B0 {
    s32 field_00;
    s32 count_04;
    s32 field_08;
    u8 **data_0C;
    s32 *param_10;
    s32 *param_14;
    u8 pad_18[0x24 - 0x18];
    u8 *data_24;
    s32 relocated_28;
} Bank8012C4B0;

/* 0x13C-byte voice record (see func_8012C138). */
typedef struct Voice8012C4B0 {
    u32 flags_00;
    u8 *active_04;
    u8 pad_08[0x34 - 0x8];
    s32 param_34;
    s32 param_38;
    u8 pad_3C[0x44 - 0x3C];
    s32 id_44;
    u8 pad_48[0x74 - 0x48];
    Bank8012C4B0 *owner_74;
    u8 pad_78[0x80 - 0x78];
    u8 *value_80;
    u8 pad_84[0x88 - 0x84];
    s32 param_88;
    s32 param_8C;
    u8 pad_90[0xD2 - 0x90];
    u8 enabled_D2;
    u8 pad_D3[0x13C - 0xD3];
} Voice8012C4B0;

extern Voice8012C4B0 *D_801CA6DC;
extern s32 D_801CA6F0;
typedef struct Record_8012A7C4 Record_8012A7C4;
extern Record_8012A7C4 *D_801CA6F8;
extern void func_8012C300(void **slots, void *base, s32 count);
extern s32 func_8012C138(Bank8012C4B0 *def, s32 slot);
extern void func_8012C004(Voice8012C4B0 *voice);

/* Start every track of `bank` under a fresh group id: one master voice and one voice per
 * populated track. */
s32 func_8012C4B0(void *resource) {
    Bank8012C4B0 *bank = resource;
    s32 count = bank->count_04;
    s32 id;
    s32 i;
    Voice8012C4B0 *voice;
    u8 *data;

    if (bank->relocated_28 == 0) {
        bank->relocated_28 = 1;
        func_8012C300((void **)&bank->data_0C, bank, 7);
        func_8012C300((void **)bank->data_0C, bank, count);
        func_8012C300((void **)bank->param_10, bank, count);
        func_8012C300((void **)bank->param_14, bank, count);
    }
    id = D_801CA6F0++;
    voice = &D_801CA6DC[func_8012C138(bank, -1)];
    func_8012C004(voice);
    voice->enabled_D2 = 1;
    voice->owner_74 = bank;
    voice->flags_00 |= 3;
    data = bank->data_24;
    voice->id_44 = id;
    voice->active_04 = voice->value_80 = data;
    for (i = 0; i < count; i++) {
        if (bank->data_0C[i] != 0) {
            voice = &D_801CA6DC[func_8012C138(bank, i)];
            func_8012C004(voice);
            voice->enabled_D2 = 1;
            voice->owner_74 = bank;
            voice->flags_00 |= 1;
            voice->param_38 = voice->param_8C = bank->param_10[i];
            voice->param_34 = voice->param_88 = bank->param_14[i];
            data = bank->data_0C[i];
            voice->id_44 = id;
            voice->active_04 = voice->value_80 = data;
        }
    }
    D_801CA6F8 = 0;
    return id;
}
