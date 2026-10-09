#include "common.h"

typedef unsigned short u16;

char *func_80048480(u16 id);
s32 func_8005EF08(char *dst, const char *fmt, ...);

/* Formats a seconds count as hours, tens of minutes, minutes, tens of seconds, seconds. */
void func_800CB154(u32 seconds, char *formatDst)
{
    u32 hours = seconds / 3600;
    u32 minuteTens;
    u32 minutes;
    u32 secondTens;

    seconds %= 3600;
    minuteTens = seconds / 600;
    seconds %= 600;
    minutes = seconds / 60;
    seconds %= 60;
    secondTens = seconds / 10;
    seconds %= 10;
    func_8005EF08(formatDst, func_80048480(0x456), hours, minuteTens, minutes, secondTens, seconds);
}
