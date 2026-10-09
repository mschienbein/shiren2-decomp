#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct ALHeap ALHeap;
typedef u32 DmaCallback801303A0(u32 address, s32 size, void *state);
typedef DmaCallback801303A0 *DmaInit801303A0(void **state);
typedef struct {
    u8 pad00[0xC]; void *state0C; void *state10; u8 pad14[0x14];
    DmaCallback801303A0 *dma28; void *dmaState2C; u32 field30;
    s32 field34; s32 field38; void *field3C; void *state40; float ratio44;
    s32 field48; s32 field4C; s32 field50; void *state54;
    s16 field58; s16 field5A; s16 field5C; s16 field5E; s16 field60;
    s16 field62; s16 field64; s16 field66; s16 field68; s16 field6A;
    s16 field6C; s16 field6E; s32 field70; s32 field74; s32 field78;
    void *field7C; void *field80; s32 field84;
} Voice801303A0;
extern void *func_8002AB40(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);

void func_8012DF08(Voice801303A0 *voice, DmaInit801303A0 *dmaInit, void *heap)
{
    voice->state0C = func_8002AB40(0, 0, heap, 1, 0x20);
    voice->state10 = func_8002AB40(0, 0, heap, 1, 0x20);
    voice->dma28 = dmaInit(&voice->dmaState2C);
    voice->field34 = 0;
    voice->field38 = 1;
    voice->field3C = 0;
    voice->state40 = func_8002AB40(0, 0, heap, 1, 0x20);
    voice->field4C = 0;
    voice->field50 = 1;
    voice->field48 = 0;
    voice->ratio44 = 1.0f;
    voice->state54 = func_8002AB40(0, 0, heap, 1, 0x50);
    voice->field78 = 1;
    voice->field84 = 0;
    voice->field5A = 1;
    voice->field68 = 1;
    voice->field6E = 1;
    voice->field5C = 1;
    voice->field5E = 1;
    voice->field60 = 0;
    voice->field62 = 0;
    /* This rate pair is assigned twice; the compiler keeps both +0x64
     * stores and drops the first +0x66 store as dead. */
    voice->field66 = 1;
    voice->field64 = 0;
    voice->field66 = 1;
    voice->field64 = 0;
    voice->field70 = 0;
    voice->field74 = 0;
    voice->field58 = 0;
    voice->field7C = 0;
    voice->field80 = 0;
}
