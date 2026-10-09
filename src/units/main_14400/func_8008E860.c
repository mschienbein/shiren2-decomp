#include "common.h"

typedef unsigned char u8;

/* Buffered ROM reader state (func_8008DD10 init, func_8008DD1C open, func_8008DEC0 close). */
typedef struct {
    u8 active;
    u8 pad1[3];
    s32 value4;
    s32 size8;
    u8 *bufferC;
} Reader;

typedef struct {
    u32 source;
    s32 size;
} Region;

typedef struct {
    s32 tag0;
    Region regions[3];
    s32 pad1C;
} Chunk;

typedef struct Handlers Handlers;
typedef struct Obj Obj;

extern Handlers D_8013FF7C;

void func_8008DD10(Reader *reader);
s32 func_8008DD1C(Reader *reader, u32 source, s32 size);
s32 func_8008E4B0(Reader *reader, Handlers *table, u32 total, Obj *ctx);
void func_8008DEC0(Reader *reader);

s32 func_8008E860(Obj *obj, Chunk *chunk) {
    Reader reader;
    Region *region = &chunk->regions[0];
    s32 result;

    func_8008DD10(&reader);
    result = func_8008DD1C(&reader, region->source, region->size);
    if (result == 0) {
        if (func_8008E4B0(&reader, &D_8013FF7C, region->size, obj)) {
            result = -1;
        }
    }
    func_8008DEC0(&reader);
    return result;
}
