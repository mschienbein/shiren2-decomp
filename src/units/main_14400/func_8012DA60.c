#include "common.h"

typedef unsigned char u8;

/* Audio command word pair. */
typedef struct {
    u32 w0;
    u32 w1;
} AudioCommand;

typedef struct Voice8012DA60 Voice8012DA60;

typedef struct {
    u8 pad00[0x14];
    s32 count;
    s32 capacity;
    Voice8012DA60 **voices;
} VoiceList8012DA60;

typedef struct {
    u8 pad00[0x34];
    VoiceList8012DA60 *voiceList;
} Player8012DA60;

extern Player8012DA60 *D_80148D84;
extern AudioCommand *func_8012E040(Voice8012DA60 *voice, s32 sampleCount, AudioCommand *cmd);

/* Synthesis callback: clear the mix buffer, then emit every voice's commands. */
AudioCommand *func_8012DA60(s32 sampleCount, AudioCommand *commands) {
    AudioCommand *cmd = commands;
    VoiceList8012DA60 *list = D_80148D84->voiceList;
    Voice8012DA60 **voices = list->voices;
    s32 i;

    {
        AudioCommand *clear = cmd++;

        clear->w0 = 0x020007C0;
        clear->w1 = 0x2E0;
    }
    for (i = 0; i < list->count; i++) {
        cmd = func_8012E040(voices[i], sampleCount, cmd);
    }
    return cmd;
}
