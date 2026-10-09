#include "common.h"
typedef struct { unsigned char active_00; unsigned char pad_01[3]; s32 cursor_04; s32 size_08; void *buffer_0C; } RomReader;
typedef struct { u32 rom_base; } Archive;
extern unsigned char D_01A56E60[];
void func_8008DD10(RomReader *reader);
void func_8006AAF0(void *destination, u32 device_address, s32 size);
s32 func_8008DD1C(RomReader *reader, u32 rom_source, s32 size);
s32 func_8008E354(RomReader *reader, Archive *archive);
void func_8008DEC0(RomReader *reader);
void func_8008E318(void *archive);
s32 func_8008E264(Archive *archive, s32 alternate)
{
    RomReader reader;
    s32 count;
    s32 result;
    func_8008DD10(&reader);
    if (alternate == 0) {
        archive->rom_base = (u32)D_01A56E60;
    } else {
        archive->rom_base = 0x01FCC000;
    }
    /* The DMA API encodes a cartridge device address, not a CPU object pointer. */
    func_8006AAF0(&count, archive->rom_base + 4, 4);
    result = func_8008DD1C(&reader, archive->rom_base, count * 8 + 8);
    if (result == 0) {
        result = func_8008E354(&reader, archive);
    }
    func_8008DEC0(&reader);
    if (result != 0) {
        func_8008E318(archive);
    }
    return result;
}
