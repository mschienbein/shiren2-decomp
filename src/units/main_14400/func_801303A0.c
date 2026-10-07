#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct Node801303A0 { struct Node801303A0 *next; u8 pad4[0x18]; } Node801303A0;
typedef struct { u8 pad0[0x8]; s32 field_8; u8 padC[0x80]; } Voice801303A0;
typedef struct { u32 w0, w1; } AudioCommand801303A0;
typedef AudioCommand801303A0 *AudioCallback801303A0(s32 sampleCount, AudioCommand801303A0 *commands);
typedef u32 DmaCallback801303A0(u32 address, s32 size, void *state);
typedef DmaCallback801303A0 *DmaInit801303A0(void **state);
typedef struct { u8 pad0[0x4]; AudioCallback801303A0 *callback; u8 pad8[0xC]; } Driver801303A0;
typedef struct {
    u8 pad0[0x14];
    s32 count;
    s32 capacity;
    Voice801303A0 **voices;
    void *field_20;
    u8 pad24[0x20];
} VoiceList801303A0;
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
    DmaInit801303A0 *field_24;
    void *heap;
    Node801303A0 *freeNodes;
    Driver801303A0 *driver;
    VoiceList801303A0 *voiceList;
    s32 voiceCount;
    u8 pad3C[0x4];
    s32 field_40;
    s32 field_44;
    s32 *field_48;
    s32 field_4C;
} Player801303A0;
typedef struct {
    s32 field_0;
    s32 voiceCount;
    s32 nodeCount;
    u8 padC[0x4];
    DmaInit801303A0 *field_10;
    void *heap;
    s32 field_18;
    u8 useAlt;
} Config801303A0;
extern Player801303A0 *D_80148D84;
void *func_8002AB40(u8 *file, s32 line, void *heap, s32 count, s32 size);
void *func_80130930(s16 index, Config801303A0 *cfg, void *heap);
AudioCallback801303A0 func_8012F350;
AudioCallback801303A0 func_8012DA60;
void func_800326AC(Voice801303A0 *voice, s32 *list);
void func_8012DF08(Voice801303A0 *voice, DmaInit801303A0 *dmaInit, void *heap);
void func_801303A0(Config801303A0 *cfg) {
    void *heap = cfg->heap;
    VoiceList801303A0 *list;
    Driver801303A0 *driver;
    Voice801303A0 *voice;
    Voice801303A0 *voices;
    Node801303A0 *node;
    Node801303A0 *nodes;
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
    D_80148D84->voiceList = func_8002AB40(0, 0, heap, 1, sizeof(VoiceList801303A0));
    D_80148D84->voiceList->count = 0;
    D_80148D84->voiceList->capacity = cfg->voiceCount;
    D_80148D84->voiceList->voices = func_8002AB40(0, 0, heap, cfg->voiceCount, 4);
    driver = func_8002AB40(0, 0, heap, 1, sizeof(Driver801303A0));
    D_80148D84->driver = driver;
    if (cfg->useAlt) {
        D_80148D84->voiceList->field_20 = func_80130930(0, cfg, heap);
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
    voices = func_8002AB40(0, 0, heap, cfg->voiceCount, sizeof(Voice801303A0));
    for (i = 0; i < cfg->voiceCount; i++) {
        voice = &voices[i];
        func_800326AC(voice, &D_80148D84->field_4);
        voice->field_8 = 0;
        func_8012DF08(voice, D_80148D84->field_24, heap);
        list = D_80148D84->voiceList;
        list->voices[list->count++] = voice;
    }
    nodes = func_8002AB40(0, 0, heap, cfg->nodeCount, sizeof(Node801303A0));
    D_80148D84->freeNodes = 0;
    for (i = 0; i < cfg->nodeCount; i++) {
        node = &nodes[i];
        node->next = D_80148D84->freeNodes;
        D_80148D84->freeNodes = node;
    }
    D_80148D84->heap = heap;
}
