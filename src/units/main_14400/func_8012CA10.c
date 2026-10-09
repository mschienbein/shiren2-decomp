#include "common.h"

typedef unsigned char u8;

typedef struct ALHeap ALHeap;

typedef s32 (*ParamProc)(void *filter, s32 paramID, void *param);

/* 0x2C-byte filter whose parameter handler sits at 0x28. */
typedef struct {
    u8 pad0[0x28];
    ParamProc set_param;
} Filter;

typedef struct {
    u8 pad0[0x20];
    void *entry;
} Config;

/* Six heap tables allocated at init (element sizes 0x28, 2, 0x34, 0x20, 0x30, 8). */
typedef struct {
    void *table_28;
    void *table_02;
    void *table_34;
    void *table_20;
    void *table_30;
    void *table_08;
} Tables;

extern Tables D_801DFF64;

void *func_8002AB40(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
s32 func_80030658(void *filter, s32 paramID, void *param);
s32 func_8012CE40(void);
s32 func_8012CE84(void);
void func_8012CB48(void *entry);

void func_8012CA10(Filter **out, Config *config, ALHeap *heap) {
    Filter *filter = func_8002AB40(0, 0, heap, 1, sizeof(Filter));

    *out = filter;
    filter->set_param = func_80030658;
    D_801DFF64.table_28 = func_8002AB40(0, 0, heap, func_8012CE40(), 0x28);
    D_801DFF64.table_02 = func_8002AB40(0, 0, heap, func_8012CE84(), 2);
    D_801DFF64.table_34 = func_8002AB40(0, 0, heap, 1, 0x34);
    D_801DFF64.table_20 = func_8002AB40(0, 0, heap, 1, 0x20);
    D_801DFF64.table_30 = func_8002AB40(0, 0, heap, 1, 0x30);
    D_801DFF64.table_08 = func_8002AB40(0, 0, heap, 1, 8);
    func_8012CB48(config->entry);
}
