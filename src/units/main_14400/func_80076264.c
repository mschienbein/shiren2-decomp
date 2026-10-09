#include "common.h"

/* One original file: func_80076264 through func_80076734 share the initialized flag
 * D_8013D8E4 (gas fills the early-return `j` delay slot of func_80076264 with its %lo store
 * only when the datum is defined in this file). */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef float f32;

typedef struct { s8 id, duration; s16 value; } AnimFrame;
typedef struct { s32 count; AnimFrame *frames; } Anim;
typedef struct { u8 pad0[3]; u8 animation; } Metadata;
typedef struct { Metadata *metadata; u8 fields4[4]; } AnimEntry;

/* 0xB0-byte entity record. */
typedef struct {
    s16 id;
    s16 kind;
    u8 field4;
    s8 action5;
    u8 level6, mode7, hidden8, state9;
    u16 flagsA;
    s16 x;
    u8 padE[2];
    s16 z;
    u8 pad12[0x2C];
    u8 frame3E;
    u8 pad3F[3];
    u8 step42;
    s8 timer43;
    u8 pad44[6];
    s16 field_4A;
    u8 pad4C[0x64];
} Entity;

/* 0x2E-byte zone record; a negative count marks an overflowed zone. */
typedef struct {
    s8 count;
    s8 pad1;
    s16 pos[3][3];
    s16 rot[3][3];
    s16 time[3];
    s16 lastTime;
} Zone;

typedef struct { f32 x; f32 y; f32 z; } Vec3f;

extern Entity D_801DEAB4[];
extern Zone D_801A79E8[];
extern Anim D_8014D1B4[];
extern s32 D_8013D8CC;
extern s32 D_8013D8E0;
s32 D_8013D8E4 = 1;
extern s32 D_8013D8E8;
extern s32 D_8013D8F0;
extern s32 D_8013D8FC;
extern s32 D_801E4E70;

s32 func_80041FF8(void);
AnimEntry *func_80074784(s32 kind, s32 level);
u32 func_8002A9B0(void);
s32 func_800751B4(s32 id, s32 flags);
void func_80074D88(Entity *self, Anim *animation, s32 ticks);
/* All six parameters are optional f32 out-pointers (each store is guarded by bnel reg,zero). */
void func_80061AB8(f32 *arg0, f32 *arg1, f32 *minX, f32 *minZ, f32 *maxX, f32 *maxZ);
s32 func_80074114(void);
void func_800593F8(Vec3f *pos);
s32 func_8005B2CC(void);
void func_8005ADAC(s32 arg0, s32 arg1, s32 arg2);
s32 func_800627C4(void);
void func_80077498(s32 arg0);
void func_80076F48(s32 arg0);
s32 func_80084E20(void);
void func_8006E6D8(void);

void func_80076264(s32 mode) {
    s32 i;
    if (D_8013D8E4 != 0) {
        if (!(func_80041FF8() & 255) && D_8013D8E0 == 1) {
            D_8013D8E4 = 1;
            return;
        }
        for (i = 0; i < 30; i++) {
            Entity *self = &D_801DEAB4[i];
            AnimEntry *entry;
            Anim *animation;
            s32 ticks;
            if (self->kind == -1 || self->hidden8 == 1 || self->mode7 != 1) continue;
            entry = func_80074784(self->kind, self->level6);
            switch (self->state9) {
            case 0:
                animation = &D_8014D1B4[entry->metadata->animation];
                ticks = 0;
                break;
            case 2:
                ticks = 0;
                if (self->kind == 0x17) animation = &D_8014D1B4[15];
                else animation = &D_8014D1B4[entry->metadata->animation];
                break;
            case 1:
                if (self->kind == 0x17) { animation = &D_8014D1B4[14]; ticks = 1; }
                else { animation = &D_8014D1B4[13]; ticks = 0; }
                break;
            case 4:
                ticks = 1;
                animation = &D_8014D1B4[entry->metadata->animation];
                if (self->action5 == 0 && (self->flagsA & 0x4000) && D_801A79E8[i].count <= 0 && self->frame3E == 8) continue;
                break;
            case 3:
                ticks = mode == 2 ? 6 : 2;
                animation = &D_8014D1B4[entry->metadata->animation];
                break;
            case 6:
                ticks = 3;
                animation = &D_8014D1B4[entry->metadata->animation];
                if (self->action5 == 0 && (self->flagsA & 0x4000) && D_801A79E8[i].count <= 0 && self->frame3E == 8) continue;
                break;
            case 7:
                ticks = 6;
                animation = &D_8014D1B4[entry->metadata->animation];
                if (self->action5 == 0 && (self->flagsA & 0x4000) && D_801A79E8[i].count <= 0 && self->frame3E == 8) continue;
                break;
            case 5:
                ticks = 2;
                animation = &D_8014D1B4[entry->metadata->animation];
                if (self->action5 == 0 && (self->flagsA & 0x4000) && D_801A79E8[i].count <= 0 && self->frame3E == 8) continue;
                break;
            default:
                ticks = 2;
                animation = &D_8014D1B4[entry->metadata->animation];
                if (self->action5 == 0 && (self->flagsA & 0x4000) && D_801A79E8[i].count <= 0 && self->frame3E == 8) continue;
                break;
            }
            if (self->action5 == 1) {
                switch (self->kind) {
                case 0x17:
                    animation = &D_8014D1B4[16];
                    ticks = 2;
                    if (animation->frames[self->step42].id == 0 && (func_8002A9B0() & 63) == 0) {
                        self->timer43 = (s8)(func_8002A9B0() % 127 + 30) % 127;
                    }
                    break;
                case 0x1B:
                    ticks = 2;
                    animation = &D_8014D1B4[18];
                    if (self->step42 >= 3) func_800751B4(i, (self->flagsA & 0x4000) | ticks);
                    break;
                }
            }
            func_80074D88(self, animation, ticks);
        }
    }
    D_8013D8E4 = 1;
}

void func_800765AC(void) {
    s32 i;
    for (i = 0; i < 30; i++) {
        D_801A79E8[i].count = 0;
        D_801A79E8[i].lastTime = -1;
    }
}

s32 func_800765E8(s32 index, s32 x, s32 y, s32 z, s32 rx, s32 ry, s32 rz, s32 time)
{
    Zone *entry = &D_801A79E8[index];
    s32 slot;

    if (entry->count >= 0) {
        if (entry->count >= 3) {
            slot = -1;
            entry->count = 3;
        } else {
            slot = entry->count++;
        }
    } else {
        slot = -1;
    }

    if (slot == -1) {
        entry->rot[2][0] = rx;
        entry->rot[2][1] = ry;
        entry->rot[2][2] = rz;
        entry->time[2] = time;
    } else {
        entry->pos[slot][0] = x;
        entry->pos[slot][1] = y;
        entry->pos[slot][2] = z;
        entry->rot[slot][0] = rx;
        entry->rot[slot][1] = ry;
        entry->rot[slot][2] = rz;
        entry->time[slot] = time;
    }
    entry->lastTime = time;
    return 1;
}

void func_800766A8(s32 index, s32 value) {
    if (D_801A79E8[index].count == 0) {
        if (D_801DEAB4[index].field_4A != value) {
            D_801DEAB4[index].field_4A = value;
        }
    } else {
        D_801A79E8[index].lastTime = value;
    }
}

void func_80076714(s32 value) {
    if (D_8013D8E4 != value) {
        D_8013D8E4 = value;
    }
}

static inline s32 clampMin(s32 value, s32 limit) {
    return (value < limit) ? limit : value;
}

static inline s32 clampMax(s32 value, s32 limit) {
    return (value > limit) ? limit : value;
}

void func_80076734(s32 force, s32 arg1) {
    Vec3f pos;
    f32 minX;
    f32 minZ;
    f32 maxX;
    f32 maxZ;
    s32 ok = 0;
    s32 n = 1;
    s32 idx;
    u32 off; /* byte offset of this zone; kept separate to match codegen */
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 cx;
    s32 cz;
    s32 i;
    s32 j;
    s32 k;
    s32 count;
    Zone *zone;
    Entity *actor;

    if (D_8013D8CC == 0) {
        return;
    }
    func_80061AB8(0, 0, &minX, &minZ, &maxX, &maxZ);
    D_8013D8E4 = 0;
    idx = func_80074114();
    off = idx * sizeof(Zone);
    func_800593F8(&pos);
    if (((Zone *)((u8 *)D_801A79E8 + off))->count != 0) {
        if ((u8)func_80041FF8() == 0 && func_8005B2CC() == 0) {
            func_8005ADAC(8, 1, 1);
        }
    } else {
        ok = 1;
        switch ((u32)func_800627C4()) {
        case 1:
            left = -10;
            right = 10;
            top = -10;
            bottom = 8;
            cx = (s32)pos.x >> 5;
            cz = (s32)pos.z >> 5;
            break;
        case 3:
            cx = D_801DEAB4[idx].x >> 7;
            cz = D_801DEAB4[idx].z >> 7;
            if (D_801E4E70 == 0 || D_8013D8F0 == 1 || D_8013D8FC == 1) {
                left = -5;
                right = 5;
                top = -5;
                bottom = 4;
            } else {
                left = (s32)((minX - 32.0f) * 0.03125f) - cx;
                right = (s32)((maxX + 32.0f) * 0.03125f) - cx;
                top = (s32)((minZ - 32.0f) * 0.03125f) - cz;
                bottom = (s32)((maxZ + 32.0f) * 0.03125f) - cz;
                left = clampMin(left, -5);
                top = clampMin(top, -5);
                right = clampMax(right, 5);
                bottom = clampMax(bottom, 4);
            }
            break;
        case 2:
        default:
            left = -5;
            right = 5;
            top = -5;
            bottom = 4;
            cx = D_801DEAB4[idx].x >> 7;
            cz = D_801DEAB4[idx].z >> 7;
            break;
        }
        for (i = 0; i < 30; i++) {
            zone = &D_801A79E8[i];
            actor = &D_801DEAB4[i];
            if (actor->kind == -1) {
                continue;
            }
            if (zone->count == 0) {
                continue;
            }
            if (D_801DEAB4[i].hidden8 == 1) {
                continue;
            }
            count = (zone->count < 0) ? 1 : zone->count;
            for (j = 0; j < count; j++) {
                if ((zone->pos[j][0] >= cx + left && zone->pos[j][0] <= cx + right
                     && zone->pos[j][1] >= cz + top && zone->pos[j][1] <= cz + bottom)
                    || (zone->rot[j][0] >= cx + left && zone->rot[j][0] <= cx + right
                        && zone->rot[j][1] >= cz + top && zone->rot[j][1] <= cz + bottom)) {
                    ok = 0;
                    break;
                }
            }
            if (ok == 0) {
                break;
            }
        }
    }
    if ((u8)func_80041FF8() != 0) {
        ok = 0;
    }
    func_80077498(0);
    if (force != 0 || ok != 0) {
        D_8013D8E4 = force ? 1 : 2;
        func_80076F48(8);
    } else {
        n = 1;
        if (D_8013D8E8 == 1) {
            n = 2;
        }
        j = 1;
        while (1) {
            D_8013D8E4 = 1;
            for (k = 0; k < n; k++) {
                func_80076F48(j);
                j++;
            }
            if (arg1 != 0) {
                func_80084E20();
            }
            if (j >= 9) {
                break;
            }
            func_8006E6D8();
        }
    }
    func_80077498(1);
    func_8006E6D8();
    func_800765AC();
}
