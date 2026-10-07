#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/*
 * The four 10-byte slots at D_801D4CDC (same layout as func_80131A70, a
 * sibling handler in table 0x80148E44). D_801D4CDE is the splat label of
 * slot 0's y halfword, not an object: index the real table.
 */
typedef struct {
    u16 x;
    u16 y;
    u16 counter;
    u8 flag;
    u8 pad7[3];
} Slot;

/* Prefix view of the request record (func_80131A70): +0 slot index. */
typedef struct {
    u8 slot;
} Request;

typedef struct {
    u8 pad0[0xC];
    Request *request;
} Task;

extern Slot D_801D4CDC[4];

s32 func_80131AFC(Task *task)
{
    D_801D4CDC[task->request->slot].y = 0;
    return 0;
}
