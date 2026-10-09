#include "common.h"

typedef unsigned char u8;
typedef struct Queue Queue;
typedef struct { unsigned short button; signed char stick_x, stick_y; u8 error; } OSContPad;
typedef struct { u8 pad00[0xC]; OSContPad *data; } Message;
extern Queue D_801E028C, D_801E4E80;
extern s32 func_80027730(Queue *queue);
extern long func_8002FEA0(Queue *queue, void **message, long flags);
extern long func_80031D50(Queue *queue, void *message, long flags);
extern void func_800277B8(OSContPad *data);

static inline s32 poll_controller(OSContPad *data)
{
    s32 result = func_80027730(&D_801E028C);
    if (result != 0) return result;
    func_8002FEA0(&D_801E028C, 0, 1);
    func_80031D50(&D_801E4E80, 0, 1);
    func_800277B8(data);
    func_8002FEA0(&D_801E4E80, 0, 1);
    return 0;
}

s32 func_80130CB4(Message *message)
{
    return poll_controller(message->data);
}
