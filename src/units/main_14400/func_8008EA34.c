#include "common.h"

typedef unsigned char u8;

/* Read stream over a ROM block or a RAM buffer (see func_8008E0C4). */
typedef struct {
    u8 state;
    u8 pad1[3];
    u32 pos;
    u32 size;
    u8 *base;
} Stream;

typedef struct {
    u32 id;
    u32 size;
} ChunkHeader;

/* 20-byte entry of the slot's file table. */
typedef struct {
    u32 offset;
    u8 pad4[0x10];
} FileEntry;

typedef struct {
    u8 pad0[3];
    u8 index;          /* 0x03 */
    u8 pad4[0x10];
    FileEntry *files;  /* 0x14 */
} Slot;

typedef struct {
    s32 compressed;    /* 0x00 */
    u8 pad4[0x10];
    u32 romBase;       /* 0x14: cartridge address of the archive */
} Archive;

typedef struct ChunkTable ChunkTable;

extern s32 D_801D85CC;
extern ChunkTable D_8013FFBC;

void func_8008DD10(Stream *stream);
void func_8008ECA8(Slot *slot, s32 index);
s32 func_8008DD1C(Stream *stream, u32 romAddr, s32 size);
u32 func_8008E0C4(Stream *stream, void *dst, u32 len);
void func_8008DEC0(Stream *stream);
s32 func_8008DF04(Stream *stream);
void *func_80091450(u32 size);
u8 *func_8008E22C(Stream *stream, u32 index);
void func_800D8E60(u8 *src, u8 *dst);
/* The second argument becomes the stream base pointer (Stream.base). */
void func_8008DE64(Stream *stream, u8 *base, u32 size);
s32 func_8008E4B0(Stream *stream, ChunkTable *table, u32 size, Slot *slot);

/* Load the slot's "EGRF" file from the archive and dispatch its chunks. */
s32 func_8008EA34(Slot *slot, Archive *archive)
{
    Stream stream;
    ChunkHeader header;
    u32 addr;
    u32 size;
    u32 length;
    u8 *buffer;
    s32 result;

    func_8008DD10(&stream);
    D_801D85CC = 0;
    func_8008ECA8(slot, slot->index);
    addr = archive->romBase + slot->files[slot->index].offset;
    result = func_8008DD1C(&stream, addr, 8);
    if (result != 0) {
        goto done;
    }
    func_8008E0C4(&stream, &header, 8);
    func_8008DEC0(&stream);
    if (header.id != 0x45475246) {
        result = -1;
        goto done;
    }
    size = header.size;
    result = func_8008DD1C(&stream, addr + 8, (s32)size);
    if (result != 0) {
        goto done;
    }
    if (archive->compressed) {
        length = func_8008DF04(&stream);
        buffer = func_80091450(length);
        if (buffer == 0) {
            goto fail;
        }
        func_800D8E60(func_8008E22C(&stream, 4), buffer);
        func_8008DEC0(&stream);
        func_8008DE64(&stream, buffer, length);
        size = length;
    }
    if (func_8008E4B0(&stream, &D_8013FFBC, size, slot)) {
fail:
        result = -1;
    }
done:
    func_8008DEC0(&stream);
    return result;
}
