#include "common.h"

typedef struct { u32 w0, w1; } AudioCommand;
/* Same slot type as the driver callbacks installed by func_801303A0 (func_8012F350, func_8012DA60). */
typedef AudioCommand *AudioCallback(s32 sampleCount, AudioCommand *commands);
typedef struct {
    unsigned char pad0[4];
    AudioCallback *callback;
} Driver;
typedef struct {
    unsigned char pad0[0x30];
    Driver *driver;
} Player;

extern Player *D_80148D84;

/* Clear the mix buffer, let the driver render into it, then mix both output channels. */
AudioCommand *func_80130980(s32 sampleCount, AudioCommand *commands)
{
    AudioCommand *out;

    {
        AudioCommand *cmd = commands++;
        cmd->w0 = 0x020004E0; /* A_CLEARBUFF 0x4E0 */
        cmd->w1 = 0x2E0;
    }
    out = D_80148D84->driver->callback(sampleCount, commands);
    {
        AudioCommand *cmd = out++;
        cmd->w0 = 0x0C007FFF; /* A_MIXER gain 0x7FFF */
        cmd->w1 = 0x07C004E0;
    }
    {
        AudioCommand *cmd = out++;
        cmd->w0 = 0x0C007FFF;
        cmd->w1 = 0x09300650;
    }
    return out;
}
