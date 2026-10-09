#include "common.h"

typedef unsigned char u8;
typedef void *ProbeMessage;
typedef struct ProbeMessageQueue ProbeMessageQueue;
typedef ProbeMessageQueue OSMesgQueue;
typedef struct OSThread OSThread;
typedef struct ProbeControllerStatus {
    unsigned short type;
    u8 status, error;
} ProbeControllerStatus;
extern void func_80027EA0(ProbeMessageQueue *, ProbeMessage *, signed long);
extern void func_80031E90(s32 event, OSMesgQueue *, void *message);
extern signed long func_80027910(ProbeMessageQueue *, u8 *, ProbeControllerStatus *);
extern void func_80033048(const char *format, ...);
extern void func_80027ED0(OSThread *, s32, void (*)(void *), void *, void *, s32);
extern void func_80131CC0(void *argument);
extern void func_80032B50(OSThread *thread);
extern ProbeMessageQueue D_801E028C;
extern ProbeMessage D_801CA970[8];
extern OSThread D_801CA990;
extern const char D_80160990[];
/* Controller thread stack: the 0x4000 bytes at 0x801CAB40..0x801CEB40 that follow
 * the 0x1B0-byte thread in the splat bss block D_801CA990 (bound by
 * PROVIDE(D_801CAB40)). D_801CEB40 labels the next object, not this stack. */
extern unsigned long long D_801CAB40[0x4000 / sizeof(unsigned long long)];

u8 func_80131E54(void)
{
    u8 found_channels;
    ProbeControllerStatus status[4];
    func_80027EA0(&D_801E028C, D_801CA970, 8);
    func_80031E90(5, &D_801E028C, 0);
    if (func_80027910(&D_801E028C, &found_channels, status))
        func_80033048(D_80160990);
    func_80027ED0(&D_801CA990, 0x208, func_80131CC0, 0, &D_801CAB40[0x4000 / sizeof(unsigned long long)], 0x73);
    func_80032B50(&D_801CA990);
    return found_channels;
}
