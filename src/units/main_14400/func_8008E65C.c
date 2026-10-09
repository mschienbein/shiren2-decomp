#include "common.h"
typedef unsigned char u8;
typedef struct { u8 bytes[0x10]; } Stream;
typedef struct { u32 words[3]; } Header;
typedef struct { u32 tag, size; } Chunk;
typedef struct { s32 kind; u32 base_offset, base_length, shared_offset, shared_length, graph_offset, graph_length; } Layout;
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_8008DD10(Stream *stream);
extern s32 func_8008DD1C(Stream *stream, u32 device_offset, s32 count);
extern u32 func_8008E0C4(Stream *stream, void *dst, u32 count);
extern void func_8008DEC0(Stream *stream);
s32 func_8008E65C(Layout *out, u32 device_offset, u32 length) {
    Header header;
    Chunk chunk;
    Stream stream;
    u32 chunks_offset = device_offset + 12;
    u32 offset;
    s32 shared = 0, graphs = 0, bases = 0;
    s32 result;
    /* ODD_C: single-pass error block grouping stream setup, FORM-header reading and the
     * chunk enumeration; it also shapes the prologue scheduling and the original
     * saved-register choice. */
    do {
        func_8006A810((u8 *)out, 0, 0x1C);
        func_8008DD10(&stream);
        result = func_8008DD1C(&stream, device_offset, 12);
        if (result != 0) break;
        func_8008E0C4(&stream, &header, 12);
        func_8008DEC0(&stream);
        if (header.words[2] == 0x45534646) out->kind = 0;
        else out->kind = 1;
        for (offset = 0; offset < length; offset += 8 + chunk.size) {
            u32 address = chunks_offset + offset;
            if (offset + 8 >= length) break;
            result = func_8008DD1C(&stream, address, 8);
            if (result != 0) break;
            func_8008E0C4(&stream, &chunk, 8);
            func_8008DEC0(&stream);
            switch (chunk.tag) {
            case 0x53485246:
                if (shared) result = -1;
                else {
                    out->shared_offset = address;
                    out->shared_length = chunk.size + 8;
                    shared = 1;
                }
                break;
            case 0x45475246:
                if (graphs >= 8) result = -1;
                else {
                    if (!graphs) out->graph_offset = address;
                    graphs++;
                    out->graph_length += 8 + chunk.size;
                }
                break;
            default:
                if (bases >= 3 || shared || graphs) result = -1;
                else {
                    if (!bases) out->base_offset = address;
                    bases++;
                    out->base_length += 8 + chunk.size;
                }
                break;
            }
            if (result != 0) break;
        }
    } while (0);
    return result;
}
