#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
/* ODD_C: the byte error at +4 is also copied as a halfword with its
 * padding (ROM 8005887C/80058888), matching Raw.field_04 in the other TUs. */
typedef struct {
    u16 button;
    s8 stick_x, stick_y;
    union { struct { u8 error; u8 padding; } bytes; u16 field_04; } status;
} Pad80058680;
typedef struct { u16 type; u8 status; u8 errno; } Status80058680;
typedef struct { Pad80058680 pad; u16 pressed; u16 held; u16 last; } Input80058680;
typedef struct Queue Queue;
extern Queue D_801E00D4;
extern s32 D_8013A9D0;
extern s32 D_801652E8;
extern void *D_801652EC[5];
extern Pad80058680 D_801D2588[4];
extern Status80058680 D_801D2C10[4];
extern Input80058680 D_80163118;
void func_8006C350(void *buf, void *queue, u8 count);
void func_801315F0(void (*callback)(s32));
long func_8002FEA0(void *queue, void *msg, long flags);
long func_80031D50(void *queue, void *msg, long flags);
void func_80130A00(Pad80058680 *pads);
void func_801315D0(void);
void func_80058990(void);
s32 func_80058A20(s32 a);

/* The scheduler calls this notification callback with an unused integer argument. */
static void func_80058964(s32 unused);

static inline void Stick_deadzone(s8 *axis) {
    s32 v = *axis;
    if (v > 0) {
        if (v < 8) *axis = 0;
        else if (v < 0x57) *axis -= 7;
        else *axis = 0x50;
    } else {
        v = -v;
        if (v < 8) *axis = 0;
        else if (v < 0x57) *axis += 7;
        else *axis = -0x50;
    }
}

/* OS thread entry supplies an unused argument. */
void func_80058680(void *unused) {
    void *buf[4];
    s32 changed = 0;
    s32 i;
    func_8006C350(buf, &D_801E00D4, 1);
    func_801315F0(func_80058964);
    while (1) {
        s32 *msg;
        Pad80058680 *pad;
        func_8002FEA0(&D_801E00D4, &msg, 1);
        switch (*msg) {
        case 0:
            D_801652E8 = 1;
            continue;
        case 100:
            break;
        default:
            continue;
        }
        func_80130A00(D_801D2588);
        for (i = 0, pad = D_801D2588; i < 4; i++, pad++) {
            if (pad->status.bytes.error != 0) {
                pad->button = 0;
                pad->stick_x = 0;
                pad->stick_y = 0;
            }
            Stick_deadzone(&pad->stick_x);
            Stick_deadzone(&pad->stick_y);
            if (pad->status.bytes.error != D_801D2C10[i].errno) changed = 1;
        }
        if (changed) {
            changed = 0;
            func_801315D0();
            func_80058990();
        }
        pad = &D_801D2588[func_80058A20(0x10)];
        D_80163118.pad = *pad;
        D_80163118.pressed = pad->button & ~D_80163118.last;
        D_80163118.held |= pad->button;
        if (pad->button & 0x800) D_80163118.held &= ~0x400;
        else if (pad->button & 0x400) D_80163118.held &= ~0x800;
        if (pad->button & 0x200) D_80163118.held &= ~0x100;
        else if (pad->button & 0x100) D_80163118.held &= ~0x200;
        D_801652E8 = 0;
        D_80163118.last = pad->button;
        for (i = 0; i < 5; i++) {
            if (D_801652EC[i] != 0) {
                func_80031D50(D_801652EC[i], 0, 0);
                D_801652EC[i] = 0;
            }
        }
    }
}

static void func_80058964(s32 unused) {
    func_80031D50(&D_801E00D4, &D_8013A9D0, 0);
}
