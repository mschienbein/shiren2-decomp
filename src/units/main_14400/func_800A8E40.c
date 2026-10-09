#include "common.h"

typedef struct {
    s32 fields_00[10];
    short adjust_28;
    short field_2A;
    void (*method_2C)(void *, s32, void *);
} ReadTable;

typedef struct Stream {
    s32 fields_00[6];
    ReadTable *table_18;
} Stream;

typedef struct {
    s32 fields_00[10];
    short adjust_28;
    short field_2A;
    void (*method_2C)(void *, Stream *);
} ObjectTable;

typedef struct {
    s32 fields_00[9];
    ObjectTable *table_24;
} Object;

extern s32 D_80142B00;
extern unsigned char D_80147620[];
extern const char D_80153650[];
extern const unsigned char D_8015488C[8];
extern unsigned char D_801C51A4[];
extern void func_800CA4E8(Stream *, const void *);
extern void func_800C56D4(void *);
extern void func_800C573C(void *);
extern Object *func_800A83D0(s32, unsigned char);

static inline s32 bit_at(unsigned char *bits, s32 index) {
    s32 result = 0;
    if (bits[index >> 3] & D_8015488C[index & 7]) {
        result = 1;
    }
    return result;
}

void func_800A8E40(Stream *stream) {
    unsigned char values[2];
    s32 i;
    ReadTable *table;
    func_800CA4E8(stream, D_80153650);
    table = stream->table_18;
    table->method_2C((unsigned char *)stream + table->adjust_28, 4, D_801C51A4);
    i = 0;
    func_800C56D4(D_80147620);
    for (;;) {
        s32 used;
        Object *object;
        ObjectTable *methods;
        if (i >= 29) break;
        used = bit_at(D_801C51A4, i);
        if (used) {
            table = stream->table_18;
            table->method_2C((unsigned char *)stream + table->adjust_28, 1, &values[0]);
            table = stream->table_18;
            table->method_2C((unsigned char *)stream + table->adjust_28, 1, &values[1]);
            D_80142B00 = i;
            object = func_800A83D0(values[0], values[1]);
            methods = object->table_24;
            methods->method_2C((unsigned char *)object + methods->adjust_28, stream);
        }
        ++i;
    }
    D_80142B00 = -1;
    func_800C573C(D_80147620);
}
