#include "common.h"
/* D_801541F8 + 0x2C is the void stream read func_800CA668(receiver, count, destination). */
typedef struct { unsigned char pad00[0x28]; short adjustment; unsigned short pad2A; void (*read)(void *, s32, void *); } Methods;
typedef struct { unsigned char pad00[0x18]; Methods *methods; } Reader;
extern void func_800CA4E8(Reader *reader, const void *section);
extern const char D_80153B0C[];
extern unsigned char D_8014313C[5], D_80143144[0xA0], D_80143114[0x28];
extern s32 D_80143110;
void func_800B0DCC(Reader *reader) {
    unsigned char count;
    func_800CA4E8(reader, D_80153B0C);
    reader->methods->read((unsigned char *)reader + reader->methods->adjustment, 5, D_8014313C);
    reader->methods->read((unsigned char *)reader + reader->methods->adjustment, 0xA0, D_80143144);
    reader->methods->read((unsigned char *)reader + reader->methods->adjustment, 1, &count);
    D_80143110 = count;
    reader->methods->read((unsigned char *)reader + reader->methods->adjustment, 0x28, D_80143114);
}
