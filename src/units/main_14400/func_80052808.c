#include "common.h"

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
typedef struct { short type; Playback playback; } State;
extern State D_80161650;
s32 func_8012A4E4(s32 id);

void func_80052808(void)
{
    func_8012A4E4(D_80161650.playback.handle);
}
