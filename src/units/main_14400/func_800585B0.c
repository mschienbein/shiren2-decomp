#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long long u64;
typedef void *Message;
typedef struct Thread Thread;
typedef struct {
    Thread *receive_waiters;
    Thread *send_waiters;
    long valid_count;
    long first;
    long capacity;
    Message *messages;
} Queue;
typedef struct OSThread OSThread;
/* Twelve-byte raw and sixteen-byte filtered input records (views of func_80058C00). */
typedef struct { u16 buttons_00; u8 x_02, y_03; u16 field_04, buttons_06, buttons_08, previous_0A; } Raw;
typedef struct { u16 field_00; u8 x_02, y_03; u16 field_04, pressed_06, repeat_08, accumulated_0A, previous_0C; u8 delay_0E; } Input;

extern Raw D_80163118;
extern Input D_80163124;
/* 0x2000-byte stack of the input thread; it ends where the thread object
 * D_80165138 begins (linker alias D_80163138 proposed for its base). */
extern u64 D_80163138[0x400];
extern OSThread D_80165138;
extern s32 D_801652E8;
extern Queue *D_801652EC[5];
extern Queue D_801E00D4;
extern Message D_801D85AC[8];

void func_80058680(void *arg);
u8 *func_8006A810(u8 *dst, s32 value, s32 count);
void func_80027EA0(Queue *queue, Message *messages, long capacity);
u8 func_80130A40(void);
void func_80058990(void);
void func_80027ED0(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 priority);
void func_80032B50(OSThread *t);

void func_800585B0(void) {
    s32 i;

    D_801652E8 = 0;
    func_8006A810((u8 *)&D_80163118, 0, sizeof(D_80163118));
    func_8006A810((u8 *)&D_80163124, 0, sizeof(D_80163124));
    for (i = 0; i < 5; i++) {
        D_801652EC[i] = 0;
    }
    func_80027EA0(&D_801E00D4, D_801D85AC, 8);
    func_80130A40();
    func_80058990();
    func_80027ED0(&D_80165138, 0x209, func_80058680, 0, D_80163138 + 0x400, 0x72);
    func_80032B50(&D_80165138);
}
