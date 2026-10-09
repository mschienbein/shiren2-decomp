#include "common.h"

/*
 * Task-scheduler state file: func_80084A68..func_80084CD4 and the initialized words
 * D_8013E900..D_8013E910 they own. One translation unit, because the original places
 * %lo(D_8013E900) stores in the jr delay slots of func_80084A68/func_80084A78 and of
 * func_80084CD4 (gas fills a delay slot with a %lo access only for a symbol defined in
 * the same file). The bodies of func_80084A84..func_80084CD4 are the accepted canonical
 * units, with their task-record views unified into one Task type.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef void (*TaskFn)(void *);

/* 0x74-byte task record; 0xAE records at D_801BA380. */
typedef struct {
    TaskFn handler_0;
    s16 mode_4;
    u16 id_6;
    s16 field_8;
    s16 field_A;
    u8 padC[0x10 - 0x0C];
    u16 field_10;
    u8 pad12[0x14 - 0x12];
    s32 value_14;
    u8 pad18[0x5C - 0x18];
    s32 y_5C[3];
    s32 x_68[3];
} Task;

typedef struct {
    u16 first;
    u16 second;
} Pair;

s32 D_8013E900 = 0;    /* current scheduler mode */
s32 D_8013E904 = 0;    /* current value (func_80084A84/func_80084A90) */
s32 D_8013E908 = 0;    /* reset by func_80084904 */
s32 D_8013E90C = 1;    /* flag (func_80084A9C/func_80084AA8) */
s32 D_8013E910 = 0;    /* read by func_80084CC8 */

extern Task D_801BA380[];
extern Pair D_801BF258[];
extern s32 D_801BF2C0;
extern s32 D_801BF2C4;
extern s32 D_801BF2C8;
extern s32 D_8013E914;

void func_800843D8(void *task);
void func_8008AF00(void *task);
void func_80084FD0(s32 a0);
void func_80084904(void);

void func_80084A68(void)
{
    D_8013E900 = 1;
}

void func_80084A78(void)
{
    D_8013E900 = 0;
}

s32 func_80084A84(void)
{
    return D_8013E904;
}

void func_80084A90(s32 value)
{
    D_8013E904 = value;
}

void func_80084A9C(s32 flag)
{
    D_8013E90C = flag;
}

s32 func_80084AA8(void)
{
    return D_8013E90C;
}

Task *func_80084AB4(s32 index)
{
    return &D_801BA380[index];
}

s32 func_80084AD8(s32 start, TaskFn handler)
{
    s32 count = 0;
    s32 id = start;

    while (id >= 0) {
        Task *task = func_80084AB4(id);

        if (task->handler_0 == func_800843D8 || task->handler_0 == func_8008AF00) {
            id = task->field_10;
        } else if (task->handler_0 == handler) {
            count++;
            break;
        }
        if (id == task->id_6) {
            return count;
        }
    }
    return count;
}

void func_80084B80(void)
{
    if (D_801BF2C4 != 0) {
        D_801BF2C4--;
        D_8013E900 = D_801BF258[D_801BF2C4].first;
        D_801BF2C0 = D_801BF258[D_801BF2C4].second;
    }
}

u16 func_80084BCC(void)
{
    s32 i = 0;
    Task *p = D_801BA380;

    for (; i < 0xAE; i++, p++) {
        if (!p->handler_0) {
            return i;
        }
    }
    return 0xAE;
}

s32 func_80084C00(s32 count, TaskFn handler, s32 y, s32 x, s32 plane)
{
    s32 i;
    Task *entry;

    for (i = 0; i < count; i++) {
        entry = &D_801BA380[i];
        if (entry->handler_0 == handler && y == entry->y_5C[plane] && x == entry->x_68[plane]) {
            return entry->value_14;
        }
    }
    return -1;
}

s32 func_80084C60(s32 count, TaskFn handler, u32 key)
{
    s32 index;

    for (index = count - 1; index >= 0; --index) {
        Task *task = &D_801BA380[index];
        if (task->handler_0 == handler && task->value_14 == key) {
            return index;
        }
    }
    return -1;
}

s32 func_80084CB8(void)
{
    return D_801BF2C0;
}

s32 func_80084CC8(void)
{
    return D_8013E910;
}

s32 func_80084CD4(TaskFn handler)
{
    s32 id = func_80084BCC();
    Task *entry;
    s32 savedCount;
    s32 savedMode;

    if (id == 0xAE) {
        func_80084FD0(func_80084A84());
        savedCount = D_801BF2C8;
        savedMode = D_8013E900;
        func_80084904();
        D_8013E900 = savedMode;
        func_80084A90(0);
        D_801BF2C8 = savedCount;
        id = func_80084BCC();
    }
    entry = &D_801BA380[id];
    entry->handler_0 = handler;
    entry->id_6 = id;
    if (D_8013E900 != 1) {
        entry->mode_4 = 0;
    } else {
        entry->mode_4 = 2;
        if (D_8013E914 != 0) {
            entry->field_10 = D_801BA380[D_801BF2C0].field_10;
            D_8013E914 = 0;
        } else {
            entry->field_10 = (u16)D_801BF2C0;
        }
    }
    entry->field_A = 0;
    entry->field_8 = 0;
    D_801BF2C0 = id;
    D_801BF2C8++;
    return id;
}
