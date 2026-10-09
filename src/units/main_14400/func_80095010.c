#include "common.h"
typedef struct { const void *field_0; unsigned char field_4[0x10]; } Reader;
extern const unsigned char D_80151DF8[24];
extern void func_80048728(void *);
extern void func_800D8FA8(void *object);
void func_80095010(Reader *reader, s32 mode) {
    reader->field_0 = D_80151DF8;
    func_80048728(reader->field_4);
    if (mode & 1) func_800D8FA8(reader);
}
