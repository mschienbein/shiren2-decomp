#include "common.h"

typedef unsigned char u8;
typedef struct ALHeap ALHeap;
typedef struct AudioConfig AudioConfig;
typedef struct AudioEffect AudioEffect;
/* The allocation at 0x8012C7C0 reserves 0x44 bytes: header plus eight pointers. */
typedef struct EffectBank {
    u8 header[0x24];
    AudioEffect *effects[8];
} EffectBank;
typedef struct AudioState {
    u8 pad_00[0x34];
    EffectBank *bank_34;
} AudioState;
extern AudioState *D_80148D84;
/* Original callee saves a1 as the configuration and a2 as ALHeap. */
extern void func_8012CA10(AudioEffect **destination, AudioConfig *config, ALHeap *heap);

AudioEffect *func_8012C9C0(short index, AudioConfig *config, ALHeap *heap)
{
    func_8012CA10(&D_80148D84->bank_34->effects[index], config, heap);
    return D_80148D84->bank_34->effects[index];
}
