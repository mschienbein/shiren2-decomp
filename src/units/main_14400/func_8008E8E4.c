#include "common.h"

typedef unsigned char u8;
typedef struct { u8 state; u8 pad_01[3]; u32 position, size; u8 *base; } Stream;
typedef struct { u32 device_address; u32 size; } RomChunk;
typedef struct { s32 compressed; u8 pad_04[8]; RomChunk chunk_0C; } Resource;
typedef struct { u32 magic; u32 size; } ChunkHeader;
typedef struct { u32 count; const void *entries; } ChunkRegistry;
extern ChunkRegistry D_8013FF8C;
extern void func_8008DD10(Stream *stream);
extern s32 func_8008DD1C(Stream *stream, u32 device_address, s32 size);
extern u32 func_8008E0C4(Stream *stream, void *destination, u32 size);
extern void func_8008DEC0(Stream *stream);
extern s32 func_8008DF04(void *stream);
extern void *func_80091450(u32 size);
extern u8 *func_8008E22C(Stream *stream, u32 position);
extern void func_800D8E60(u8 *source, u8 *destination);
/* 8008DE64 stores a real allocation pointer at stream + 0xC. */
extern void func_8008DE64(Stream *stream, void *buffer, u32 size);
extern s32 func_8008E4B0(Stream *stream, ChunkRegistry *registry, u32 size, void *context);

/* Load the resource's "SHRF" chunk (optionally compressed) and dispatch its sub-chunks. */
s32 func_8008E8E4(void *context, Resource *resource)
{
    Stream stream;
    ChunkHeader header;
    RomChunk *chunk = &resource->chunk_0C;
    s32 result;
    u32 value; /* chunk ROM address, later reused for the unpacked size */
    u32 size;
    func_8008DD10(&stream);
    result = 0;
    if (chunk->size != 0) {
        /* ODD_C: single-pass error block routing header, buffer and dispatch failures to the
         * shared stream close; it also shapes the header-load scheduling. */
        do {
            value = chunk->device_address;
            result = func_8008DD1C(&stream, value, 8);
            if (result != 0) {
                break;
            }
            func_8008E0C4(&stream, &header, 8);
            func_8008DEC0(&stream);
            result = -1;
            if (header.magic != 0x53485246) {
                break;
            }
            size = header.size;
            result = func_8008DD1C(&stream, value + 8, size);
            if (result != 0) {
                break;
            }
            if (resource->compressed != 0) {
                u8 *buffer;

                value = func_8008DF04(&stream);
                buffer = func_80091450(value);
                if (buffer == 0) {
                    result = -1;
                    break;
                }
                func_800D8E60(func_8008E22C(&stream, 4), buffer);
                func_8008DEC0(&stream);
                func_8008DE64(&stream, buffer, value);
                size = value;
            }
            if (func_8008E4B0(&stream, &D_8013FF8C, size, context) != 0) {
                result = -1;
            }
        } while (0);
    }
    func_8008DEC0(&stream);
    return result;
}
