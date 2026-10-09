#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Queue Queue;
typedef struct Thread Thread;
typedef union { u32 word; struct { u16 high, id; } parts; } Command;
typedef struct Message { Command command; Queue *reply; s32 result; void *payload; } Message;
typedef s32 (*Handler)(void *);
typedef struct HandlerNode { struct HandlerNode *next; Handler *methods; u16 group; u8 count; } HandlerNode;
typedef struct QueueNode { struct QueueNode *next; Queue *queue; u8 flags; } QueueNode;
extern Queue D_801D9334;
extern HandlerNode *D_80148E70;
extern void func_80027EA0(Queue *, void **, long);
extern void func_8006C350(QueueNode *, void *, u8);
extern long func_8002FEA0(Queue *, void **, long);
extern long func_80031D50(Queue *, void *, long);
extern void func_8006C3D8(QueueNode *, u8);
extern void func_80032C70(Thread *);

void func_80131CC0(void *thread_argument)
{
    QueueNode node;
    void *messages[8];
    void *received;
    s32 handled = 0;
    /* osCreateThread supplies thread_argument, which this entry does not use. */
    func_80027EA0(&D_801D9334, messages, 8);
    func_8006C350(&node, &D_801D9334, 1);
    for (;;) {
        Message *message;
        HandlerNode **cursor;
        func_8002FEA0(&D_801D9334, &received, 1);
        message = received;
        cursor = &D_80148E70;
        switch (message->command.word) {
        case 0:
            while (*cursor) {
                if ((*cursor)->methods[0]) handled = (*cursor)->methods[0](received);
                if (handled) break;
                cursor = &(*cursor)->next;
            }
            break;
        case 0x7F00:
            func_80031D50(message->reply, 0, 1);
            func_8006C3D8(&node, 0);
            func_80032C70(0);
            func_8006C3D8(&node, 1);
            break;
        default: {
            u16 group = ((Message *)received)->command.parts.id & 0xFF00;
            u16 method = ((Message *)received)->command.parts.id & 0xFF;
            while (*cursor) {
                if ((*cursor)->group == group) {
                    if (method < (*cursor)->count)
                        ((Message *)received)->result = (*cursor)->methods[method](received);
                    if (((Message *)received)->reply)
                        func_80031D50(((Message *)received)->reply, 0, 1);
                    break;
                }
                cursor = &(*cursor)->next;
            }
            break;
        }
        }
    }
}
