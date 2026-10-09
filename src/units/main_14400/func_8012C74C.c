#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef struct ALHeap ALHeap;
typedef struct AudioEffect AudioEffect;

typedef struct Node8012C74C { struct Node8012C74C *next; u8 pad4[0x18]; } Node8012C74C;
typedef struct { u8 pad0[0x8]; s32 field_8; u8 padC[0x80]; } Voice8012C74C;
typedef struct { u32 w0, w1; } AudioCommand8012C74C;
typedef AudioCommand8012C74C *AudioCallback8012C74C(s32 sampleCount, AudioCommand8012C74C *commands);
typedef u32 DmaCallback8012C74C(u32 address, s32 size, void *state);
typedef DmaCallback8012C74C *DmaInit8012C74C(void **state);
typedef struct { u8 pad0[0x4]; AudioCallback8012C74C *callback; u8 pad8[0xC]; } Driver8012C74C;
typedef struct {
    u8 pad0[0x14];
    s32 count;
    s32 capacity;
    Voice8012C74C **voices;
    AudioEffect *field_20;
    u8 pad24[0x20];
} VoiceList8012C74C;
typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    DmaInit8012C74C *field_24;
    ALHeap *heap;
    Node8012C74C *freeNodes;
    Driver8012C74C *driver;
    VoiceList8012C74C *voiceList;
    s32 voiceCount;
    u8 pad3C[0x4];
    s32 field_40;
    s32 field_44;
    s32 *field_48;
    s32 field_4C;
} Player8012C74C;
typedef struct AudioConfig {
    s32 field_0;
    s32 voiceCount;
    s32 nodeCount;
    u8 padC[0x4];
    DmaInit8012C74C *field_10;
    ALHeap *heap;
    s32 field_18;
    u8 useAlt;
} AudioConfig;
extern Player8012C74C *D_80148D84;
void *func_8002AB40(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
AudioEffect *func_8012C9C0(s16 index, AudioConfig *config, ALHeap *heap);
AudioCallback8012C74C func_8012F350;
AudioCallback8012C74C func_8012DA60;
void func_800326AC(Voice8012C74C *voice, s32 *list);
void func_8012DF08(Voice8012C74C *voice, DmaInit8012C74C *dmaInit, void *heap);
/* Synthesizer init (same shape as func_801303A0): voice table, driver callback,
 * voices and the free-node pool, all from the configured heap. */
void func_8012C74C(AudioConfig *cfg) {
    ALHeap *heap = cfg->heap;
    VoiceList8012C74C *list;
    Driver8012C74C *driver;
    Voice8012C74C *voice;
    Voice8012C74C *voices;
    Node8012C74C *node;
    Node8012C74C *nodes;
    s32 i;

    D_80148D84->field_0 = 0;
    D_80148D84->voiceCount = cfg->voiceCount;
    D_80148D84->field_20 = 0;
    D_80148D84->field_1C = 0;
    D_80148D84->field_40 = cfg->field_18;
    D_80148D84->field_44 = 0xB8;
    D_80148D84->field_24 = cfg->field_10;
    D_80148D84->field_48 = 0;
    D_80148D84->field_4C = 1;
    D_80148D84->voiceList = func_8002AB40(0, 0, heap, 1, sizeof(VoiceList8012C74C));
    D_80148D84->voiceList->count = 0;
    D_80148D84->voiceList->capacity = cfg->voiceCount;
    D_80148D84->voiceList->voices = func_8002AB40(0, 0, heap, cfg->voiceCount, 4);
    driver = func_8002AB40(0, 0, heap, 1, sizeof(Driver8012C74C));
    D_80148D84->driver = driver;
    if (cfg->useAlt) {
        D_80148D84->voiceList->field_20 = func_8012C9C0(0, cfg, heap);
        D_80148D84->driver->callback = func_8012F350;
    } else {
        driver->callback = func_8012DA60;
    }
    D_80148D84->field_4 = 0;
    D_80148D84->field_8 = 0;
    D_80148D84->field_14 = 0;
    D_80148D84->field_18 = 0;
    D_80148D84->field_C = 0;
    D_80148D84->field_10 = 0;
    voices = func_8002AB40(0, 0, heap, cfg->voiceCount, sizeof(Voice8012C74C));
    for (i = 0; i < cfg->voiceCount; i++) {
        voice = &voices[i];
        func_800326AC(voice, &D_80148D84->field_4);
        voice->field_8 = 0;
        func_8012DF08(voice, D_80148D84->field_24, heap);
        list = D_80148D84->voiceList;
        list->voices[list->count++] = voice;
    }
    nodes = func_8002AB40(0, 0, heap, cfg->nodeCount, sizeof(Node8012C74C));
    D_80148D84->freeNodes = 0;
    for (i = 0; i < cfg->nodeCount; i++) {
        node = &nodes[i];
        node->next = D_80148D84->freeNodes;
        D_80148D84->freeNodes = node;
    }
    D_80148D84->heap = heap;
}
