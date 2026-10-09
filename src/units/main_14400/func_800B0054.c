#include "common.h"

typedef struct {
    short adjust;
    short field_02;
    void (*method)(void *, s32, void *);
} WriteMethod;

typedef struct {
    s32 fields_00[6];
    WriteMethod write_18;
} StreamTable;

typedef struct {
    s32 fields_00[6];
    StreamTable *table_18;
} Stream;

typedef struct {
    s32 fields_00[8];
    short adjust_20;
    short field_22;
    void (*method_24)(void *, Stream *);
} EntryTable;

typedef struct {
    unsigned char field_00;
    unsigned char type_01;
    unsigned char fields_02[6];
    EntryTable *table_08;
    unsigned char fields_0C[36];
} Entry;

typedef struct {
    Entry *entries_00;
    unsigned char *bits_04;
    s32 count_08;
} Collection;

/* "ItemTbl": NUL-terminated type-name tag in rodata, read byte-wise by func_800CA584. */
extern const char D_80153B00[];
extern const unsigned char D_8015488C[8];
extern void func_800CA4A4(Stream *, const char *name);

void func_800B0054(Collection *collection, Stream *stream) {
    unsigned char type;
    s32 i;
    s32 offset;
    WriteMethod *writer;
    func_800CA4A4(stream, D_80153B00);
    writer = &stream->table_18->write_18;
    writer->method((unsigned char *)stream + writer->adjust,
                   (collection->count_08 + 7) / 8, collection->bits_04);
    i = 0;
    offset = 0;
    for (;;) {
        if (i >= collection->count_08) {
            return;
        }
        if (collection->bits_04[i >> 3] & D_8015488C[i & 7]) {
            Entry *entry = (Entry *)((unsigned char *)collection->entries_00 + offset);
            StreamTable *table = stream->table_18;
            EntryTable *methods;
            type = entry->type_01;
            table->write_18.method((unsigned char *)stream + table->write_18.adjust, 1, &type);
            methods = entry->table_08;
            methods->method_24((unsigned char *)entry + methods->adjust_20, stream);
        }
        offset += 0x30;
        ++i;
    }
}
