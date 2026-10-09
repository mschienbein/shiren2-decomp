#include "common.h"

typedef unsigned char u8;
typedef struct Thread Thread;
typedef struct Synth Synth;
typedef u32 (*DmaProc)(u32 address, s32 length, void *state);
typedef DmaProc (*DmaFactory)(void **state);
typedef struct { s32 physicalVoices; s32 virtualVoices; s32 updates; s32 unused0C; DmaFactory dma; void *heap; s32 frequency; u8 effects; s32 *params; } SynthConfig;
typedef struct { s32 field00; s32 voices04; s32 field08; s32 priority0C; u8 pad10[0x18]; s32 updates28; u32 frequency2C; s32 commands30; s32 frames34; s32 buffers38; s32 bufferSize3C; } Config;
typedef struct { void *samples; s32 count; } AudioBuffer;
extern Synth D_801DE9B4;
extern Thread D_801CA780;
extern void *D_801CA938;
extern AudioBuffer *D_801CA934;
extern u8 *D_801CA930;
extern DmaFactory func_8012CF30(s32 count, s32 bufferSize);
extern s32 func_80025EE0(u32 frequency);
extern u8 *func_8012D894(void);
extern void func_8012C6E0(Synth *synth, SynthConfig *config);
extern s32 func_8012D940(s32 scale, s32 frequency, u32 rate, s32 margin);
extern void *func_8012D84C(s32 size);
extern void func_8012D688(void *arg);
extern void func_80027ED0(Thread *thread, s32 id, void (*entry)(void *), void *arg, void *stack, s32 priority);
extern void func_80032B50(Thread *thread);

void func_8012D550(Config *config, s32 rate, s32 effect)
{
    SynthConfig synthConfig;
    s32 sampleCount;
    u32 i = 0;
    u8 *stack;
    synthConfig.physicalVoices = synthConfig.virtualVoices = config->voices04;
    synthConfig.updates = config->updates28;
    synthConfig.dma = func_8012CF30(config->buffers38, config->bufferSize3C);
    synthConfig.effects = effect;
    synthConfig.frequency = func_80025EE0(config->frequency2C);
    synthConfig.heap = func_8012D894();
    func_8012C6E0(&D_801DE9B4, &synthConfig);
    sampleCount = func_8012D940(config->frames34, synthConfig.frequency, rate, 30);
    D_801CA938 = func_8012D84C(config->commands30 * 8);
    D_801CA934 = func_8012D84C(3 * sizeof(AudioBuffer));
    do {
        D_801CA934[i].samples = func_8012D84C(sampleCount * 4);
        ++i;
    } while (i < 3);
    stack = func_8012D84C(0x2000);
    D_801CA930 = stack;
    func_80027ED0(&D_801CA780, 3, func_8012D688, 0, stack + 0x2000, config->priority0C);
    func_80032B50(&D_801CA780);
}
