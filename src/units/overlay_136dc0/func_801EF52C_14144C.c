#include "func_801EF52C_14144C/types.h"
extern u8 D_801F5248_147168[], D_801F5228_147148[32], D_801F5218_147138[], D_801F5214_147134[], D_801F5210_147130[];
extern const char D_801F6054_147F74[];
extern void func_800CA4A4(void *stream, void *name);

void func_801EF52C_14144C(S10Stream *stream)
{
    S10StreamVTable *table;
    func_800CA4A4(stream, (void *)D_801F6054_147F74);
    table = stream->vtable18;
    table->write1C((u8 *)stream + table->writeDelta18, 0x40, D_801F5248_147168);
    table = stream->vtable18;
    table->write1C((u8 *)stream + table->writeDelta18, 0x20, D_801F5228_147148);
    table = stream->vtable18;
    table->write1C((u8 *)stream + table->writeDelta18, 0x10, D_801F5218_147138);
    table = stream->vtable18;
    table->write1C((u8 *)stream + table->writeDelta18, 4, D_801F5214_147134);
    table = stream->vtable18;
    table->write1C((u8 *)stream + table->writeDelta18, 4, D_801F5210_147130);
}
