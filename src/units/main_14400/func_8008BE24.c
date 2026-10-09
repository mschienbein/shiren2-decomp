#include "common.h"

/* One original file: func_8008BE24..func_8008C384 share the initialized bank pointer
 * D_8013FEE4 (gas fills the original delay slots with its %lo loads/stores only when the
 * symbol is defined in this file). */

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { float values[16]; } Matrix;

/* Seven-word heap statistics record written by func_80091618. */
typedef struct {
    s32 total;
    s32 used;
    s32 largest_used;
    s32 smallest_used;
    s32 free;
    s32 largest_free;
    s32 smallest_free;
} HeapStats;

/* 0x20-byte stream slot (func_8008C6B8 initializes it, func_8008E600 opens it). */
typedef struct { u8 used_00; u8 pad_01[0x1F]; } Slot;

/* 0x78-byte channel record. */
typedef struct {
    u8 enabled_00;
    u8 active_01;
    u8 flags_02;
    u8 code_03;
    s32 field_04;
    s32 data_08;
    float value_0C;
    Matrix matrix_10;
    u8 pad_50[0x28];
} Channel;

/* Complete 0x4C8-byte bank allocated by func_8008BE24 and initialized by func_8008CF78:
 * eight slots, eight channels, then two counters. */
typedef struct {
    Slot slots[8];
    Channel channels[8];
    s32 count_4C0;
    s32 count_4C4;
} Bank;

extern const char D_80151160[];
extern s32 D_8013FEE0;
Bank *D_8013FEE4 = 0;
extern s32 D_8013FEE8;
/* Complete 16-byte archive object at .bss 0x801C33D0 (see func_8008E3B0/func_8008E600). */
extern s32 D_801C33D0[4];
extern s32 D_801D2BFC;

void *func_8006A8D8(char *name, u32 size);
s32 func_800913C0(void *buffer, u32 size);
void *func_80091450(u32 size);
void func_80091544(void *item);
void func_80091444(void);
s32 func_80091618(HeapStats *out);
void func_8008CF00(Bank *bank);
void func_8008CF78(Bank *bank);
void func_8008CFF0(Bank *bank);
void func_8008E318(void *archive);
s32 func_8008E264(void *archive, s32 alternate);
void func_8006CD14(void);
void func_8002FA20(Matrix *matrix, float x, float y, float z, float scale, float a, float b, float c);
s32 func_8008D114(Bank *bank, s32 first, s32 second);
s32 func_8008D1B0(Bank *bank);
s32 func_8008E600(Slot *slot, void *archive, u32 first, u16 second);
s32 func_8008D07C(Bank *bank, Slot *slot);
void func_8008D354(Bank *bank, s32 index);
void func_8008D2A8(Bank *bank, s32 index, s32 force);
float func_8008CE34(Channel *channel);
s32 func_8008CE58(Channel *channel);

s32 func_8008BE24(u32 size)
{
    void *object;
    s32 result;

    /* ODD_C: single-pass error block grouping the heap-reserve, bank-allocation and
     * archive-open failures before the shared return; it also shapes GCC's scheduling
     * (prologue/argument-copy order and the delay-slot use of D_8013FEE4). */
    do {
        object = func_8006A8D8((char *)D_80151160, size);
        if (object == 0) {
            result = -1;
            break;
        }
        func_800913C0(object, size);
        D_8013FEE4 = func_80091450(sizeof(Bank));
        if (D_8013FEE4 == 0) {
            result = -1;
            break;
        }
        func_8008CF00(D_8013FEE4);
        result = func_8008E264(D_801C33D0, D_8013FEE8);
        if (result == 0) {
            D_8013FEE0 = 1;
        }
    } while (0);
    return result;
}

void func_8008BEB8(void)
{
    if (D_8013FEE4) {
        func_8008CF78(D_8013FEE4);
        func_80091544(D_8013FEE4);
        D_8013FEE4 = 0;
    }
    func_8008E318(D_801C33D0);
    func_80091444();
    D_8013FEE0 = 0;
}

s32 func_8008BF14(s32 first, s32 second, s32 code, s32 enabled, s32 data, float *rotation, float scale)
{
    Matrix matrix;
    HeapStats stats;
    HeapStats *stats_pointer;
    s32 slot;
    s32 error = 0;
    s32 created = 0;
    s32 result = -1;
    u8 flags = enabled;
    s32 used;
    Channel *channel;

    if (D_8013FEE0 == 0) {
        return result;
    }
    stats_pointer = &stats;
    D_801D2BFC = 0;
    func_80091618(stats_pointer);
    used = stats.used;
    /* ODD_C: single-pass error block grouping slot acquisition, archive open and channel
     * setup before the shared slot-release cleanup; it also shapes the scheduling and the
     * original saved-register choice. */
    do {
        func_8002FA20(&matrix, 0.0f, 0.0f, 0.0f, scale, rotation[0], rotation[1], rotation[2]);
        slot = func_8008D114(D_8013FEE4, first, second);
        if (slot < 0) {
            slot = func_8008D1B0(D_8013FEE4);
            if (slot < 0) {
                error = -1;
                break;
            }
            created = 1;
            if (func_8008E600(&D_8013FEE4->slots[slot], D_801C33D0, first, second)) {
                error = -1;
                break;
            }
        }
        func_80091618(stats_pointer);
        /* Heap bytes consumed by opening the slot. */
        D_801D2BFC = stats.used - used;
        result = func_8008D07C(D_8013FEE4, &D_8013FEE4->slots[slot]);
        if (result < 0) {
            error = -1;
            break;
        }
        channel = &D_8013FEE4->channels[result];
        channel->matrix_10.values[0] = matrix.values[0];
        channel->matrix_10.values[1] = matrix.values[1];
        channel->matrix_10.values[2] = matrix.values[2];
        channel->matrix_10.values[3] = matrix.values[3];
        channel->matrix_10.values[4] = matrix.values[4];
        channel->matrix_10.values[5] = matrix.values[5];
        channel->matrix_10.values[6] = matrix.values[6];
        channel->matrix_10.values[7] = matrix.values[7];
        channel->matrix_10.values[8] = matrix.values[8];
        channel->matrix_10.values[9] = matrix.values[9];
        channel->matrix_10.values[10] = matrix.values[10];
        channel->matrix_10.values[11] = matrix.values[11];
        channel->matrix_10.values[12] = matrix.values[12];
        channel->matrix_10.values[13] = matrix.values[13];
        channel->matrix_10.values[14] = matrix.values[14];
        channel->matrix_10.values[15] = matrix.values[15];
        if (flags) {
            channel->flags_02 |= 1;
        } else {
            channel->flags_02 &= 0xFE;
        }
        channel->code_03 = code;
        channel->data_08 = data;
    } while (0);
    if (error) {
        if (created) {
            func_8008D354(D_8013FEE4, slot);
        }
        result = -1;
    }
    return result;
}

void func_8008C194(s32 index)
{
    if (D_8013FEE0 != 0) {
        func_8008D2A8(D_8013FEE4, index, 0);
    }
}

s32 func_8008C1C8(s32 index, float delta)
{
    Channel *ch;
    float target;
    float value;
    s32 result = 0;

    if (D_8013FEE0 == 0 || (ch = &D_8013FEE4->channels[index])->enabled_00 == 0) {
        return -1;
    }
    if (ch->active_01 != 0) {
        target = func_8008CE34(ch);
        if (ch->value_0C == target && delta != 0.0f) {
            ch->active_01 = 0;
            result = 1;
        } else if (ch->value_0C == 0.0f) {
            ch->value_0C = 1.0f;
        } else {
            value = ch->value_0C + delta;
            ch->value_0C = value;
            if (target < value) {
                ch->value_0C = target;
            }
        }
        if (func_8008CE58(ch) != 0) {
            result |= 2;
        }
    } else {
        result = 4;
    }
    return result;
}

void func_8008C2E0(void)
{
    if (D_8013FEE0 != 0) {
        func_8008CFF0(D_8013FEE4);
    }
}

s32 func_8008C30C(void)
{
    if (D_8013FEE0 == 0) {
        return 0;
    }
    return D_801C33D0[2];
}

void func_8008C334(s32 index)
{
    Channel *channel;
    if (D_8013FEE0 != 0) {
        channel = &D_8013FEE4->channels[index];
        if (channel->enabled_00 != 0 && channel->active_01 == 0) {
            channel->active_01 = 1;
            channel->value_0C = 0.0f;
        }
    }
}

s32 func_8008C384(s32 value)
{
    s32 old = D_8013FEE8;
    D_8013FEE8 = value;
    if (D_8013FEE0 != 0) {
        func_8006CD14();
        func_8008CF78(D_8013FEE4);
        func_8008E318(D_801C33D0);
        func_8008E264(D_801C33D0, D_8013FEE8);
    }
    return old;
}
