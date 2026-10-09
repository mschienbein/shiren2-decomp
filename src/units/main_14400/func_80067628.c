#include "common.h"
typedef unsigned char u8;
typedef union { u32 word; struct { u8 high[3]; u8 index; } bytes; } SceneChoice;
/* PI DMA consumes cartridge byte offsets, not CPU pointers, for the ROM range. */
typedef struct { u8 pad_00[3]; u8 red_03, green_04, blue_05; u8 pad_06[0xE]; u32 rom_start_14, rom_end_18; u8 *compressed_1C; u32 field_20; } SceneEntry;
typedef struct { u8 kind, subtype, flags; } Cell;
typedef struct { u8 active, count, flags; u8 payload[0x405]; } SceneChunk;
typedef struct { u8 active; u8 payload[0x13]; } SceneObject;
extern u32 *D_801D2554, *D_8013B804;
extern u8 D_8016DC11, D_8016DC12, D_8013C490;
extern SceneEntry D_8013C4FC[];
extern u8 *D_801D85A8, *D_801D2C08;
extern Cell *D_801E02A4;
extern SceneChunk *D_801D40D4;
extern SceneObject *D_801D2560;
void func_8006AAF0(void *destination, u32 device_address, s32 size);
s32 func_80083B10(u8 *source, u8 *destination);
void func_80067860(void);
void func_8006A088(u8 red, u8 green, u8 blue, u8 alpha);
void func_8006A1C8(s32 x, s32 y);
void func_80067628(SceneChoice *choice)
{
    SceneEntry *entry;
    Cell *cell;
    SceneChunk *chunk;
    SceneObject *object;
    D_8013B804 = D_801D2554;
    D_8016DC11 = choice->bytes.index;
    if (D_8016DC11 > 0x10) {
        choice->word = 0;
    }
    D_8016DC11 = choice->bytes.index;
    entry = &D_8013C4FC[D_8016DC11];
    func_8006AAF0(D_801D85A8, entry->rom_start_14, entry->rom_end_18 - entry->rom_start_14);
    func_80083B10(entry->compressed_1C, D_801D2C08);
    D_8016DC12 = 0;
    for (cell = D_801E02A4; cell < D_801E02A4 + 4104; cell++) {
        cell->kind = 1;
        cell->subtype = 0xFF;
        cell->flags = 0;
    }
    for (chunk = D_801D40D4; chunk < D_801D40D4 + 99; chunk++) {
        chunk->active = 1;
        chunk->flags = 0;
        chunk->count = 0;
    }
    D_8013C490 = 1;
    for (object = D_801D2560; object < D_801D2560 + 198; object++) {
        object->active = 0;
    }
    func_80067860();
    func_8006A088(D_8013C4FC[D_8016DC11].red_03, D_8013C4FC[D_8016DC11].green_04, D_8013C4FC[D_8016DC11].blue_05, 0);
    func_8006A1C8(0x3C5, 0x3CC);
}
