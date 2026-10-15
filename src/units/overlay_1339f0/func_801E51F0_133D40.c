#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Reconstructed Controller Pak joint, 0x801E51F0..0x801E5A24. It is
 * supported by adjacency (the alignment gaps at 0x801E51E8 and 0x801E5A24),
 * the shared status word and the single constant pool these functions
 * use; this is a reconstruction, not a proven historical file identity.
 * No text padding or overlapping data definitions. */
typedef struct {
    void *slot;
    s32 index;
    s32 result;
    s32 enabled;
} PakFile;
/* Resident func_8002E470 writes this 32-byte record: game +4, company +8,
 * extension +10 (4 bytes), game name +14 (16 bytes). */
typedef struct OSPfsState {
    u32 size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;
typedef union { char text[3]; short value; } CompanyCode;
typedef union { char text[5]; u32 value; } GameCode;
typedef struct { char text[14]; } GameName;

extern u8 *D_801E8290_136DE0;
extern u8 *D_801E8294_136DE4;
extern PakFile D_801E8298_136DE8;
extern OSPfsState D_801E82A8_136DF8[16];
extern char D_801E84A8_136FF8[17];
extern u32 D_801E7840_136390;
extern const CompanyCode D_801E7ED0_136A20;
extern const GameCode D_801E7ED4_136A24;
extern const GameName D_801E7EDC_136A2C;

void func_80042BA8(u32 address, void *buffer, u32 length, s32 read);
void *func_8006A8D8(char *name, u32 size);
void func_8006A9D4(void *address);
void func_800265E0(void *dst, s32 length);
void func_80058D70(void);
void func_80058E58(void *owner);
void func_80058DA8(void *first, void *second);
void func_80058DC4(void *obj, const void *src, unsigned char *code, s32 arg3, s32 arg4);
void func_80058DE4(void *file, s32 offset, s32 size, void *buffer, s32 flag);
void func_80058E04(void);
void func_80058E20(void *object, OSPfsState *state);
s32 func_80058E3C(void *object, u8 *name, u8 *extension);
s32 func_80083D54(u8 *first, u8 *second);

u32 D_801E7840_136390 = 0;

void func_801E51F0_133D40(u32 address, void *buffer, u32 length) {
    func_80042BA8(address, buffer, length, 1);
}

void func_801E520C_133D5C(u32 address, void *buffer, u32 length) {
    func_80042BA8(address, buffer, length, 0);
}

u8 *func_801E5228_133D78(void) {
    D_801E8290_136DE0 = func_8006A8D8("contpak", 0x12000);
    if (D_801E8290_136DE0 == 0) {
        return 0;
    }
    D_801E8294_136DE4 = D_801E8290_136DE0 + 0x9000;
    func_800265E0(D_801E82A8_136DF8, sizeof(D_801E82A8_136DF8));
    func_800265E0(&D_801E8298_136DE8, sizeof(D_801E8298_136DE8));
    D_801E7840_136390 = 0;
    func_800265E0(D_801E84A8_136FF8, sizeof(D_801E84A8_136FF8));
    return D_801E8290_136DE0;
}

void func_801E52BC_133E0C(void) {
    func_8006A9D4(D_801E8290_136DE0);
}

/* Aligned, typed views of the ROM's company/game byte strings. They are
 * immutable code identifiers; their union scalar views express big-endian
 * loads without casting a char array to an incompatible scalar object. */
const CompanyCode D_801E7ED0_136A20 = { "01" };
const GameCode D_801E7ED4_136A24 = { "NSIJ" };

void func_801E52E0_133E30(s32 unused, char *ext) {
    D_801E7840_136390 = 0x66;
    D_801E8290_136DE0 = D_801E8294_136DE4;
    func_80058D70();
    func_80058E58(&D_801E8298_136DE8);
    func_80058DA8((void *)&D_801E7ED0_136A20, (void *)&D_801E7ED4_136A24);
    func_80058DC4(&D_801E8298_136DE8, D_801E84A8_136FF8, ext, 1, 0x2800);
    if (D_801E8298_136DE8.result != 0) {
        D_801E7840_136390 = D_801E8298_136DE8.result;
    } else {
        func_80058DE4(&D_801E8298_136DE8, 0, 0x2800, D_801E8290_136DE0, 1);
        if (D_801E8298_136DE8.result != 0) {
            D_801E7840_136390 = D_801E8298_136DE8.result;
        }
    }
    func_80058E04();
}

s32 func_801E53BC_133F0C(s32 index) {
    OSPfsState *state = &D_801E82A8_136DF8[index];
    D_801E7840_136390 = 0x65;
    func_80058D70();
    func_80058E58(&D_801E8298_136DE8);
    func_80058DA8((void *)&D_801E7ED0_136A20, (void *)&D_801E7ED4_136A24);
    func_80058DC4(&D_801E8298_136DE8, state->game_name, state->ext_name, 0, state->size);
    if (D_801E8298_136DE8.result != 0) {
        D_801E7840_136390 = D_801E8298_136DE8.result;
    } else {
        func_80058DE4(&D_801E8298_136DE8, 0, state->size, D_801E8290_136DE0, 0);
        if (D_801E8298_136DE8.result != 0) {
            D_801E7840_136390 = D_801E8298_136DE8.result;
        } else {
            D_801E8294_136DE4 = D_801E8290_136DE0;
        }
    }
    func_80058E04();
    return 0;
}

void func_801E54A4_133FF4(s32 index, s32 mode) {
    OSPfsState *state = &D_801E82A8_136DF8[index];
    s32 offset;
    s32 size;
    D_801E7840_136390 = 0x65;
    func_80058D70();
    func_80058E58(&D_801E8298_136DE8);
    func_80058DA8((void *)&D_801E7ED0_136A20, (void *)&D_801E7ED4_136A24);
    func_80058DC4(&D_801E8298_136DE8, state->game_name, state->ext_name, 0, state->size);
    if (D_801E8298_136DE8.result != 0) {
        D_801E7840_136390 = D_801E8298_136DE8.result;
    } else {
        func_80058DE4(&D_801E8298_136DE8, 0, state->size, D_801E8290_136DE0, 0);
        if (D_801E8298_136DE8.result != 0) {
            D_801E7840_136390 = D_801E8298_136DE8.result;
        } else {
            D_801E8294_136DE4 = D_801E8290_136DE0;
            switch (mode) {
            case 1: size = 0x2800; offset = 0; break;
            case 2: size = 0x2800; offset = 0x2800; break;
            default: size = 0x8000; offset = 0; break;
            }
            func_801E520C_133D5C(0x08000000 | offset, D_801E8294_136DE4, size);
        }
    }
    func_80058E04();
}

void func_801E55DC_13412C(s32 index) {
    OSPfsState *state = &D_801E82A8_136DF8[index];
    D_801E7840_136390 = 0x67;
    func_80058D70();
    func_80058E58(&D_801E8298_136DE8);
    func_80058DA8(&state->company_code, &state->game_code);
    func_80058E3C(&D_801E8298_136DE8, (u8 *)state->game_name, (u8 *)state->ext_name);
    func_80058E04();
}

void func_801E5654_1341A4(s32 mode, char *ext) {
    s32 offset;
    s32 size;
    /* ODD_C: one transfer-length local carries first the whole-buffer
     * backup length and then the selected save length; the switch selects
     * into its own size/offset pair. A single variable (switch arms writing
     * the transfer length directly) measured 308 B / 14 words: the length
     * then occupies s0 instead of the ROM's a2 selection copied to s1. */
    s32 length;
    D_801E7840_136390 = 0x66;
    length = 0x8000;
    func_801E51F0_133D40(0x08000000, D_801E8290_136DE0, length);
    switch (mode) {
    case 1: size = 0x2800; offset = 0; break;
    case 2: size = 0x2800; offset = 0x2800; break;
    default: size = 0x8000; offset = 0; break;
    }
    D_801E8294_136DE4 = D_801E8290_136DE0 + offset;
    length = size;
    func_80058D70();
    func_80058E58(&D_801E8298_136DE8);
    func_80058DA8((void *)&D_801E7ED0_136A20, (void *)&D_801E7ED4_136A24);
    func_80058DC4(&D_801E8298_136DE8, D_801E84A8_136FF8, ext, 1, length);
    if (D_801E8298_136DE8.result != 0) {
        D_801E7840_136390 = D_801E8298_136DE8.result;
    } else {
        func_80058DE4(&D_801E8298_136DE8, 0, length, D_801E8294_136DE4, 1);
        if (D_801E8298_136DE8.result != 0) {
            D_801E7840_136390 = D_801E8298_136DE8.result;
        }
    }
    func_80058E04();
}

u32 func_801E5788_1342D8(void) {
    u32 value = D_801E7840_136390;
    if (value < 101) return value;
    return 0;
}

u8 *func_801E57A4_1342F4(void) {
    return D_801E8294_136DE4;
}

const GameName D_801E7EDC_136A2C = { "kRvQh[yO\x12\x0f\x89;_" };

/* ODD_C: the scan keeps a record cursor one past the 16-entry table and
 * pre-decrements both it and the file number at the top of each pass;
 * `continue` skips the clear for a recognised save. Measured standalone
 * (ovla-5654-opus runs/): this body with only the cursor initializer
 * changed to the literal `&table[16]` gives 308 B / 58 words with a for
 * loop (s_g4) and with this while loop (s_I1): table+512 is materialised
 * instead of the table base. Keeping `&table[i]` with a for loop
 * `for (i--; i != -1; i--)` is exact (s_I2), so the loop spelling is
 * immaterial. `while (--i >= 0)` measures 300 B / 72 words (s_N4). The
 * prior joint's indexed &table[i] record per pass gave 40 words. The
 * identifier pointers are block scoped inside the successful-state branch:
 * direct const-object access hoists the code loads out of the loop (s_p1),
 * and function-scope pointers keep them in s6/s7 (s_g1). */
void func_801E57B4_134304(u8 *found) {
    s32 i;
    OSPfsState *state;
    func_800265E0(found, 16);
    func_80058D70();
    func_80058E58(&D_801E8298_136DE8);
    if (D_801E8298_136DE8.enabled == 1) {
        i = 16;
        state = &D_801E82A8_136DF8[i];
        while (--i != -1) {
            state--;
            D_801E8298_136DE8.index = i;
            func_80058E20(&D_801E8298_136DE8, state);
            if (D_801E8298_136DE8.result == 0) {
                const CompanyCode *company = &D_801E7ED0_136A20;
                const GameCode *game = &D_801E7ED4_136A24;
                if (state->company_code == company->value
                    && state->game_code == game->value
                    && func_80083D54((u8 *)state->game_name,
                                     (u8 *)D_801E7EDC_136A2C.text) == 0) {
                    found[i] = state->ext_name[0] - 0x10;
                    continue;
                }
            }
            func_800265E0(state, sizeof(OSPfsState));
        }
    } else {
        D_801E7840_136390 = 1;
    }
    func_80058E04();
}

void func_801E58EC_13443C(void) {
    *(GameName *)D_801E84A8_136FF8 = D_801E7EDC_136A2C;
}

u32 func_801E5940_134490(void) {
    func_80058D70();
    func_80058E58(&D_801E8298_136DE8);
    if (D_801E8298_136DE8.enabled == 1) {
        D_801E7840_136390 = 0;
    } else {
        D_801E7840_136390 = 1;
    }
    func_80058E04();
    return D_801E7840_136390;
}

s32 func_801E59A4_1344F4(void) {
    s32 message = 0;
    switch (D_801E7840_136390) {
    /* ODD_C: ROM initializes v1=0 and cases 0/5/9/100/104 retain that
     * live default via the common return. The staged source records that
     * explicit zero assignments/returns produce 128 bytes and 31 words. */
    case 0: case 5: case 9: case 100: case 104: break;
    case 2: message = 0x50A; break;
    case 3: message = 0x50B; break;
    case 6: message = 0x50C; break;
    case 11: case 12: case 13: message = 0x50D; break;
    case 10: return -10;
    case 1: message = 0x50F; break;
    case 7: case 8: message = 0x510; break;
    case 101: case 102: message = 0x515; break;
    case 103: message = 0x516; break;
    default: message = 0x513; break;
    }
    return message;
}
