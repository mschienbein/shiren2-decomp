#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Buffered ROM reader state (func_8008DD10 init, func_8008DD1C open, func_8008DEC0 close). */
typedef struct { u8 data[0x10]; } Reader8008EBA8;
/* Chunk table filled by func_8008E65C. */
typedef struct { u8 data[0x20]; } Chunk8008EBA8;
/* IFF "FORM" header: id, payload size, form type. */
typedef struct { u32 id; u32 size; u32 type; } Header8008EBA8;
typedef struct { u8 pad0[3]; u8 field_3; u8 pad4[0xC]; u8 field_10[8]; } Obj8008EBA8;

void func_8008DD10(Reader8008EBA8 *reader);
s32 func_8008DD1C(Reader8008EBA8 *reader, u32 file, s32 size);
u32 func_8008E0C4(Reader8008EBA8 *reader, void *dst, u32 size);
void func_8008DEC0(Reader8008EBA8 *reader);
s32 func_8008E65C(Chunk8008EBA8 *chunk, u32 file, u32 size);
s32 func_8008E860(Obj8008EBA8 *obj, Chunk8008EBA8 *chunk);
s32 func_8008E8E4(Obj8008EBA8 *obj, Chunk8008EBA8 *chunk);
u8 func_8008D758(void *dst, u8 arg1);
s32 func_8008EA34(Obj8008EBA8 *obj, Chunk8008EBA8 *chunk);

/* Load an IFF "FORM" file of type "ESFF"/"ESFC" from ROM address `file`. */
s32 func_8008EBA8(Obj8008EBA8 *obj, u32 file, s32 unused, u16 arg3) {
    /* The archive entry size is supplied but not used by this loader. */
    Header8008EBA8 header;
    Chunk8008EBA8 chunk;
    Reader8008EBA8 reader;
    s32 result;

    /* ODD_C: single-pass error block short-circuiting FORM validation and the
     * shared/graph/base load sequence; it also shapes the prologue scheduling. */
    do {
        func_8008DD10(&reader);
        result = func_8008DD1C(&reader, file, 12);
        if (result != 0) {
            break;
        }
        func_8008E0C4(&reader, &header, 12);
        func_8008DEC0(&reader);
        if (header.id != 0x464F524D || (header.type != 0x45534646 && header.type != 0x45534643)) {
            result = -1;
            break;
        }
        result = func_8008E65C(&chunk, file, header.size);
        if (result != 0) {
            break;
        }
        result = func_8008E860(obj, &chunk);
        if (result != 0) {
            break;
        }
        result = func_8008E8E4(obj, &chunk);
        if (result != 0) {
            break;
        }
        obj->field_3 = func_8008D758(obj->field_10, arg3);
        result = func_8008EA34(obj, &chunk);
    } while (0);
    return result;
}
