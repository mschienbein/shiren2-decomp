#include "common.h"

/*
 * Task-scheduler state file: func_80084A68..func_80084FD0 and the initialized words
 * D_8013E900..D_8013E910 they own. One translation unit:
 * - the original places %lo(D_8013E900) stores in the jr delay slots of
 *   func_80084A68/func_80084A78 and of func_80084CD4 (gas fills a delay slot with a
 *   %lo access only for a symbol defined in the same file);
 * - likewise gas fills the beqz delay slot at 0x8008501C with func_80084FD0's
 *   `D_8013E910 = 1` store (%lo) only because D_8013E910 is defined in this file (as
 *   an extern it assembles to 300 B / 56 words: scratch/omp/wave32/s84fd0-review-astra/
 *   probes/repaired_extern.c); func_80084E20 and func_80084FD0 both drive D_8013E910.
 * The bodies of func_80084A68..func_80084CD4 are the accepted unit unchanged; those of
 * func_80084E20 and func_80084FA0 are the accepted units with func_80084E20's task view
 * mapped onto the shared Task members; func_80084FD0 is scratch/omp/wave3/a73.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef void (*TaskFn)(void *);

/* 0x74-byte task record; 0xAE records at D_801BA380. */
typedef struct {
    TaskFn handler_0;
    u16 mode_4;        /* task state: 1 run, 2 wait for parent, 3 kill, 4 finished (lhu in func_80084E20) */
    u16 id_6;
    s16 field_8;
    s16 field_A;
    u8 padC[0x0E - 0x0C];
    u16 done_E;        /* set to 1 when the task finishes */
    u16 field_10;      /* parent task index */
    u16 flags_12;      /* 0x8000: not counted as active */
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
s32 D_8013E910 = 0;    /* set while the scheduler runs (func_80084E20/func_80084FD0); read by func_80084CC8 */

extern Task D_801BA380[];
extern Pair D_801BF258[];
extern s32 D_801BF2C0;
extern s32 D_801BF2C4;
extern s32 D_801BF2C8;
extern s32 D_8013E914;
extern s32 D_8013E918;   /* frame counter (func_80084FA0) */
extern s32 D_8013E920;
extern s32 D_8013E924;

void func_800843D8(void *task);
void func_8008AF00(void *task);
void func_80084FD0(s32 a0);
void func_80084904(void);
void func_8006E6D8(void);
s32 func_8005B2CC(void);
s32 func_80054920(void);
void func_80054AC4(void);

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

s32 func_80084E20(void) {
    s32 active,i;
    D_8013E910=1;
    active=0;
    i=0;
    do {
        Task *task=&D_801BA380[i];
        TaskFn callback=task->handler_0;
        if(callback) {
            unsigned short parentIndex=task->field_10;
            unsigned short state=task->mode_4;
            Task *parent=&D_801BA380[parentIndex];
            switch(state) {
            case 1:
                callback(task);
                if(task->mode_4==4) { task->handler_0=0; task->done_E=1; }
                else if(!(task->flags_12&0x8000)) active++;
                break;
            case 2:
                if(!(task->flags_12&0x8000)) active++;
                if(parentIndex==i || (parent->done_E&1)) task->mode_4=1;
                if(task->mode_4==1) {
                    task->handler_0(task);
                    if(task->mode_4==4) { task->handler_0=0; task->done_E=1; }
                    else if(!(task->flags_12&0x8000)) active++;
                }
                break;
            case 3:
                task->handler_0=0;
                break;
            }
        }
        i++;
    } while(i<0xAE);
    D_8013E910=0;
    return active;
}

void func_80084FA0(void){func_8006E6D8(); D_8013E918++;}

void func_80084FD0(s32 a0) {
    /* ODD_C: volatile per scratch/omp/rulings/VOLATILE-LOCAL.md. The original keeps
     * the func_80084E20 result in frame slot sp+0x10 and re-reads it: sw at 0x8008500C,
     * lw at 0x80085010 (busy && a0 == 0) and lw at 0x80085030 (loop test). Those reloads
     * are why the object is volatile; GCC 2.8.1 deletes them for any non-volatile object
     * (cse.c records the store, reload_cse_noop_set_p drops a spilled pseudo's reload). */
    volatile s32 busy;

    D_8013E910 = 1;
    if (func_8005B2CC()) {
        D_8013E920 = 0;
    }
    do {
        busy = func_80084E20();
        D_8013E910 = 1;
        if (busy && a0 == 0) {
            func_80084FA0();
        }
    } while (busy);
    if (a0 == 0) {
        if (D_8013E920 != 0) {
            while (func_80054920() || func_8005B2CC()) {
                func_80084FA0();
            }
        } else {
            while (func_80054920()) {
                func_80084FA0();
            }
            if (!func_8005B2CC()) {
                D_8013E920 = 1;
            }
        }
        if (D_8013E924 != 0) {
            func_80054AC4();
        }
        D_8013E924 = 0;
    }
    D_8013E910 = 0;
}
