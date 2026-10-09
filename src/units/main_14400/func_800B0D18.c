#include "common.h"
typedef unsigned char u8;
/* D_801541F8 + 1C is the void writer func_800CA610. */
typedef struct { u8 pad_00[0x18]; short adjust_18; short reserved_1A; void (*write_1C)(void *, s32, void *); } VTable;
typedef struct { u8 pad_00[0x18]; VTable *vtable_18; } Writer;
extern void func_800CA4A4(void *writer, void *tag);
extern const char D_80153B0C[];
/* Recent-list length word; the save stores only its low byte (lbu at 0x80143113). */
extern s32 D_80143110;
extern u8 D_8014313C[5], D_80143144[0xA0], D_80143114[0x28];
void func_800B0D18(void *arg) {
    Writer *writer = arg;
    u8 value;
    func_800CA4A4(writer, (void *)D_80153B0C);
    writer->vtable_18->write_1C((u8 *)writer + writer->vtable_18->adjust_18, 5, D_8014313C);
    writer->vtable_18->write_1C((u8 *)writer + writer->vtable_18->adjust_18, 0xA0, D_80143144);
    value = D_80143110;
    writer->vtable_18->write_1C((u8 *)writer + writer->vtable_18->adjust_18, 1, &value);
    writer->vtable_18->write_1C((u8 *)writer + writer->vtable_18->adjust_18, 0x28, D_80143114);
}
