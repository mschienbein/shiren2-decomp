#include "common.h"
/* Same encoded-ROM/resident-RAM union as func_800724DC and func_80071CCC. */
typedef union { u32 segmented; void *resident; } FrameData;
typedef struct { FrameData field_0; unsigned short field_4; unsigned char field_6, field_7; u32 field_8; } Record;
typedef struct { unsigned char field_0; unsigned char field_1[3]; FrameData *field_4; unsigned char field_8; unsigned char field_9[3]; Record *field_C; } Header;
extern char D_8014C890[], D_8014C8A0[], D_8014C8B0[], D_8014C8C4[], D_8014C8D8[], D_8014C8EC[];
extern unsigned char D_5000000[], D_00E53DB0[];
extern Header *D_801D25A0[];
extern void *D_8013D510;
extern s32 D_8013D514;
extern void *func_8006A8D8(char *, u32);
extern void func_8006AAF0(void *, u32, s32);
extern s32 func_800724DC(Record *, void *), func_8007248C(u32, void *, s32);

/* Bytes for one frame: (width * (|height| + 1) + 1) >> 1, rounded up to 8. The negative
 * height case returns from its own copy of the expression; the ROM keeps the sign test
 * even though field_7 is loaded unsigned (bltzl at 0x8007128C). */
static inline s32 frame_bytes(Record *rec)
{
    s32 height = rec->field_7;
    s32 width = rec->field_6;
    if (height < 0)
        return (((width * (-height + 1) + 1) >> 1) + 7) & -8;
    return (((width * (height + 1) + 1) >> 1) + 7) & -8;
}

/* local-arithmetic-qualification: DMA accepts cartridge addresses; segment offsets and ROM-symbol addresses are encoded into that address space, not dereferenced as CPU pointers. */
void func_80071088(void)
{
    Header local;
    Header *header;
    void *buffer;
    s32 error = 0;
    s32 i, j, k, loaded;
    /* ODD_C: single-pass block; the only exit is the `if (error) break;` at its end, which
     * skips the D_8013D514 flag store. Its loop note also keeps the first allocator call's
     * argument setup after the prologue saves (ROM 0x800710B8..0x800710C4). */
    do {
        D_8013D510 = func_8006A8D8(D_8014C890, 0x2080);
        for (i = 0; i < 0x195; i++) {
            D_801D25A0[i] = 0;
            func_8006AAF0(&local, (u32)D_00E53DB0 + ((u32)D_5000000 & 0xFFFFFF) + i * 0x10, 0x10);
            if (local.field_0 & 0xC0) continue;
            header = func_8006A8D8(D_8014C8A0, 0x10);
            if (!header) { error = -1; break; }
            *header = local;
            D_801D25A0[i] = header;
            buffer = func_8006A8D8(D_8014C8B0, header->field_8 * 12);
            if (!buffer) { error = -1; break; }
            func_8006AAF0(buffer, (u32)D_00E53DB0 + ((u32)D_801D25A0[i]->field_C & 0xFFFFFF), D_801D25A0[i]->field_8 * 12);
            D_801D25A0[i]->field_C = buffer;
            buffer = func_8006A8D8(D_8014C8C4, (D_801D25A0[i]->field_0 & 0x1F) * 4);
            if (!buffer) { error = -1; break; }
            func_8006AAF0(buffer, (u32)D_00E53DB0 + ((u32)D_801D25A0[i]->field_4 & 0xFFFFFF), (D_801D25A0[i]->field_0 & 0x1F) * 4);
            D_801D25A0[i]->field_4 = buffer;
        }
        for (i = 0; i < 0x195; i++) {
            if (!D_801D25A0[i]) continue;
            header = D_801D25A0[i];
            loaded = 0;
            if (!(header->field_0 & 0xC0)) {
                for (j = 0; j < header->field_8; j++) {
                    if (header->field_C[j].field_0.segmented & 0xF000000) {
                        buffer = func_8006A8D8(D_8014C8D8, frame_bytes(&header->field_C[j]));
                        if (!buffer) { error = -1; break; }
                        if (func_800724DC(&header->field_C[j], buffer)) { error = -1; break; }
                        header->field_C[j].field_0.resident = buffer;
                    }
                    loaded = 1;
                }
                if (error) break;
            }
            if (!loaded) continue;
            for (k = 0; k < (header->field_0 & 0x1F); k++) {
                if (header->field_4[k].segmented & 0xF000000) {
                    buffer = func_8006A8D8(D_8014C8EC, 0x20);
                    if (!buffer) { error = -1; break; }
                    if (func_8007248C(header->field_4[k].segmented, buffer, 0x20)) { error = -1; break; }
                    header->field_4[k].resident = buffer;
                }
            }
            if (error) break;
        }
        if (error) break;
        D_8013D514 = 1;
    } while (0);
}
