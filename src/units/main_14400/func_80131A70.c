#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u16 x;
    u16 y;
    u8 pad4[2];
    u8 flag;
} Source;

typedef struct {
    u8 slot;
    u8 pad1[7];
    Source *source;
} Request;

typedef struct {
    u8 pad0[0xC];
    Request *request;
} Task;

typedef struct {
    u16 x;
    u16 y;
    u16 counter;
    u8 flag;
    u8 pad7[3];
} Slot;

extern Slot D_801D4CDC[];

s32 func_80131A70(Task *task) {
    Request *request = task->request;
    Source *source = request->source;

    D_801D4CDC[request->slot].flag = source->flag;
    D_801D4CDC[request->slot].y = source->y;
    D_801D4CDC[request->slot].x = source->x;
    D_801D4CDC[request->slot].counter = 0;
    return 0;
}
