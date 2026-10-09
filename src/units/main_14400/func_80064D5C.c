#include "common.h"

/* Joint unit for the original file func_80064D5C..func_80066B20: it owns the initialized
   .data at 0x8013B980..0x8013B98B (gas fills func_80064D5C's delay slot with the
   %lo(D_8013B980) store only when the symbol is defined here). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef float f32;
typedef float Matrix[4][4];

typedef struct { u32 w0, w1; } Gfx;
typedef struct { s32 m[4][4]; } Mtx;
typedef struct { float x, y, z; } Vector;

/* func_80059624 is canonically typed with Vec3i, but the camera target it copies
   (D_80165324) holds three floats; this view reads the copied words as floats. */
typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Vec3i;
typedef union {
    Vec3i words;
    struct {
        f32 x;
        f32 y;
        f32 z;
    } f;
} CameraAngles;

/* Segment header read from ROM: segmented addresses of the data start and of the menu state. */
typedef struct { u8 *start; u32 field_4; u8 *end; } Section;

typedef struct { s32 field_00; void *texture, *uv, *transform; } AnimationSet;
/* Map-segment model node. */
typedef struct {
    void *field_00;
    void *field_04;
    u8 pad_08[4];
    s16 min[3];
    s16 max[3];
    s16 priority_18;
    u8 pad_1A[0xE];
    u32 flags_28;
    u8 pad_2C[4];
    AnimationSet *animations_30;
} Model;
/* 0x2C-byte object type (D_801DE97C->records[]). */
typedef struct {
    u16 flags;
    u8 pad_02[2];
    float tx, ty, tz;
    float rx, ry, rz;
    float sx, sy, sz;
    Model **models;
} Cell;
typedef struct { Cell *records; u32 count; } CellTable;
typedef struct { float min_x, max_x, min_y, max_y, min_z, max_z; } Bounds;
/* 0x28-byte draw record (D_801D40CC[], sorted through D_801D934C[]). */
typedef struct {
    f32 depth_00;
    union { float v[6]; Bounds box; } bounds;
    Model *model_1C;
    s32 matrix_20;
    u8 used_24;
    u8 visible_25;
    u8 animation_26;
    u8 class_27;
} Slot;
typedef struct { u8 field_00[0x48]; Mtx *matrix; } State;
typedef struct { Model *model; State state; } StateRecord;

typedef struct { s32 x0, y0, x1, y1; u8 pad_10[0x10]; } Room;
typedef struct { s32 width, height; u16 *tiles; s32 room_count; Room *rooms; } MapInfo;
typedef struct { s32 x, y; } Position;
typedef struct { u16 id; u8 flags; u8 pad3; } GroupMark;
typedef struct { s32 field_00; s32 position_count; Position *positions; s32 mark_count; GroupMark *marks; } Group;
typedef struct { s32 count; Group *groups; } GroupList;
typedef struct { u16 id; u8 pad2[2]; s32 x, y, cols, rows; u16 *tiles; } Patch;
typedef struct { s32 count; Patch *patches; } PatchList;
/* Map settings block inside the relocated map section. Bytes 6..21 are indexed by a
 * terrain class: func_800669FC's only caller (func_80062C64) passes
 * func_800B200C's result, which is -1 or (flags & 0xF), so 16 entries cover it. */
typedef struct {
    u8 field_00;
    u8 bordered;
    u8 field_02;
    u8 field_03;
    u8 field_04;
    u8 field_05;
    u8 class_values_06[16];
} Settings;
struct MenuState { MapInfo *map; GroupList *groups; PatchList *patches; void *field_0C; Settings *settings; };
typedef struct MenuState MenuState;

/* 0x34-byte map entry (D_8013C084[]). */
typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    float field_0C, field_10, field_14;
    u32 data_18; /* PI ROM offset, not a CPU pointer. */
    u8 pad_1C[0x14];
    u16 *marks_30;
} Entry;
typedef struct { u32 romAddr; u8 unk4[0x30]; } TableEntry; /* romAddr: PI ROM address, not a CPU pointer */

MenuState *D_8013B980 = 0;
Entry *D_8013B984 = 0;
s32 D_8013B988 = 3;

extern u32 D_8016DC00;
extern u32 D_8016DC08;
extern s32 D_8016DB6C;
extern s32 D_8016DB70;
extern s32 D_8016DB78;
extern s32 D_8016DC0C;
extern s32 D_8013B800;
extern f32 *D_8013B804; /* float grid read by func_80062430/func_80062554 */
extern Entry D_8013C084[];
extern TableEntry D_8013C09C[];
extern Mtx D_8016DB80[2];
extern CellTable *D_801DE97C;
extern void *D_801DFF7C;
extern void *D_801D2550;
extern Mtx *D_801D9350;
extern StateRecord *D_801D2564;
extern Slot *D_801D40CC;
extern Slot **D_801D934C;
extern Gfx *D_801D2C24, *D_801DEAB0;
extern s32 D_801DE9AC, D_801DE9B0, D_801DEAAC, D_801E4E78;

void func_80030FD0(Mtx *m, float a, float x, float y, float z);
void func_8002CCC0(Mtx *m, Mtx *n, Mtx *res);
void func_80031220(Mtx *m, float x, float y, float z);
void func_80033A20(Mtx *m, float x, float y, float z);
void func_8002CFB0(Mtx *m, float x, float y, float z, float *ox, float *oy, float *oz);
void *func_80062CCC(void *value, void *base, u32 tag);
void func_80062D10(Model **head, void *base, u8 segment);
void func_80063F40(State *state);
void func_80059624(Vec3i *dst);
void func_8002CE20(float mf[4][4]);
void func_80030E70(Matrix mf, float a, float x, float y, float z);
void func_8002CBC0(float mf[4][4], float nf[4][4], float res[4][4]);
void func_8002CD40(float mf[4][4], Mtx *m);
void func_8002CE80(float mf[4][4], Mtx *m);
u32 func_800340F0(void *address);
s32 func_80041574(s32 a, s32 b);
void func_800418CC(s32 *y, s32 *x);
s32 func_8004252C(u32 index);
void func_800593F8(Vector *out);
void func_80059B18(float dst[4][4]);
s32 func_800627C4(void);
s32 func_800627F4(void);
Gfx *func_80062E88(Gfx *gfx);
Gfx *func_80062EA4(Gfx *gfx, Model *model, State *state, s32 count, void *vertices, u8 mode, s32 frame);
void func_80063FA0(void);
void func_80063FB0(Model *model, State *state);
Gfx *func_80069F14(Gfx *gfx);
Slot *func_80065500(void);
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
/* Returns 0, D_8013B980->field_0C or D_801DFF7C plus a byte offset: the selected float grid. */
f32 *func_80064A70(u32 index);

/* Rebase a segmented address from the section's ROM image to the loaded copy.
 * local-arithmetic-qualification: the stored values are ROM-image offsets, not addresses of C objects
 * (no in-bounds pointer difference exists); the u8 pointer form also emits base + offset instead of
 * the original offset + base. */
#define RELOCATE(type, section, address) ((type)((u32)(address) - (u32)(section)->start + (u32)D_801DFF7C))

void func_80064D5C(Section *section) {
    Mtx temp;
    Mtx rotation;
    float ox, oy, oz;
    u32 i;
    u32 total;
    s32 a, b, c;
    float x, y, z;
    Cell *type;
    Mtx *matrix;
    Model **list;
    Model *node;
    Slot *slot;
    Bounds *bounds;
    MenuState *state;
    MapInfo *map;
    GroupList *groups;
    PatchList *patches;
    Room *room, *room_end;
    Group *group, *group_end;
    Position *position, *position_end;
    Patch *patch, *patch_end;

    i = 0;
    D_801DE97C->records = RELOCATE(Cell *, section, D_801DE97C->records);
    D_8016DC08 = 0;
    total = 0;
    D_8013B980 = RELOCATE(MenuState *, section, section->field_4);
    for (; i < D_801DE97C->count; i++) {
        type = &D_801DE97C->records[i];
        matrix = &D_801D9350[i];
        func_80030FD0(&rotation, type->rx * (180.0 / 3.141592654), 1.0f, 0.0f, 0.0f);
        func_80030FD0(&temp, type->ry * (180.0 / 3.141592654), 0.0f, 1.0f, 0.0f);
        func_8002CCC0(&rotation, &temp, &rotation);
        func_80030FD0(&temp, type->rz * (180.0 / 3.141592654), 0.0f, 0.0f, 1.0f);
        func_8002CCC0(&rotation, &temp, &rotation);
        func_80031220(&temp, type->sx, type->sy, type->sz);
        func_8002CCC0(&temp, &rotation, matrix);
        func_80033A20(&temp, type->tx, type->ty, type->tz);
        func_8002CCC0(matrix, &temp, matrix);
        list = func_80062CCC(type->models, D_801D2550, 6);
        type->models = list;
        func_80062D10(list, D_801D2550, 6);
        while ((node = *list++) != 0) {
            total++;
            slot = func_80065500();
            bounds = &slot->bounds.box;
            if (slot == 0) continue;
            slot->matrix_20 = i;
            slot->model_1C = node;
            slot->animation_26 = 0xFF;
            for (a = 0, x = slot->model_1C->min[0]; a < 2; a++, x = slot->model_1C->max[0]) {
                for (b = 0, y = slot->model_1C->min[1]; b < 2; b++, y = slot->model_1C->max[1]) {
                    for (c = 0, z = slot->model_1C->min[2]; c < 2; c++, z = slot->model_1C->max[2]) {
                        func_8002CFB0(matrix, x, y, z, &ox, &oy, &oz);
                        if (a == 0 && b == 0 && c == 0) {
                            bounds->max_x = *(float *)bounds = ox;
                            bounds->min_y = bounds->max_y = oy;
                            bounds->min_z = bounds->max_z = oz;
                        } else {
                            if (bounds->min_x > ox) bounds->min_x = ox;
                            if (bounds->max_x < ox) bounds->max_x = ox;
                            if (bounds->min_y > oy) bounds->min_y = oy;
                            if (bounds->max_y < oy) bounds->max_y = oy;
                            if (bounds->min_z > oz) bounds->min_z = oz;
                            if (bounds->max_z < oz) bounds->max_z = oz;
                        }
                    }
                }
            }
            if (node->animations_30 != 0
                && (node->animations_30->texture != 0 || node->animations_30->uv != 0
                    || node->animations_30->transform != 0)) {
                for (a = 0; a < D_8016DC08; a++) {
                    if (D_801D2564[a].model == node) break;
                }
                if (a >= D_8016DC08) {
                    if (D_8016DC08 < D_8016DB70) {
                        slot->animation_26 = D_8016DC08;
                        D_801D2564[D_8016DC08].model = node;
                        func_80063F40(&D_801D2564[D_8016DC08].state);
                        D_8016DC08++;
                    }
                } else {
                    slot->animation_26 = a;
                }
            }
        }
    }
    if (D_8016DB6C < total) D_8016DC0C = 1;

    state = D_8013B980;
    map = state->map = RELOCATE(MapInfo *, section, state->map);
    groups = state->groups = RELOCATE(GroupList *, section, state->groups);
    patches = state->patches = RELOCATE(PatchList *, section, state->patches);
    state->field_0C = RELOCATE(void *, section, state->field_0C);
    state->settings = RELOCATE(Settings *, section, state->settings);

    map->tiles = RELOCATE(u16 *, section, map->tiles);
    map->rooms = RELOCATE(Room *, section, map->rooms);
    for (room = map->rooms, room_end = room + map->room_count; room < room_end; room++) {
        room->x0 += 10;
        room->y0 += 10;
        room->x1 += 10;
        room->y1 += 10;
    }
    groups->groups = RELOCATE(Group *, section, groups->groups);
    for (group = groups->groups, group_end = group + groups->count; group < group_end; group++) {
        group->positions = RELOCATE(Position *, section, group->positions);
        for (position = group->positions, position_end = position + group->position_count;
             position < position_end; position++) {
            position->x += 10;
            position->y += 10;
        }
        group->marks = RELOCATE(GroupMark *, section, group->marks);
    }
    patches->patches = RELOCATE(Patch *, section, patches->patches);
    for (patch = patches->patches, patch_end = patch + patches->count; patch < patch_end; patch++) {
        patch->tiles = RELOCATE(u16 *, section, patch->tiles);
        patch->x += 10;
        patch->y += 10;
    }
}

Slot *func_80065500(void)
{
    Slot *slot = D_801D40CC;
    Slot *end = slot + D_8016DB6C;
    while (slot < end) {
        if (slot->used_24 == 0) {
            slot->used_24 = 1;
            slot->visible_25 = 1;
            return slot;
        }
        slot++;
    }
    return 0;
}

void func_80065560(void) {
    Mtx view;
    Mtx world;
    CameraAngles angles;
    Matrix rotation;
    Matrix turn;
    f32 x;
    f32 y;
    Slot *rec;
    Slot *end;
    Slot **list;
    s32 lo;
    s32 hi;
    s32 last;
    s32 j;
    s32 swap;

    func_80059624(&angles.words);
    func_8002CE20(rotation);
    func_80030E70(turn, 360.0 - angles.f.y * (180.0 / 3.141592654), 0.0f, 1.0f, 0.0f);
    func_8002CBC0(rotation, turn, rotation);
    func_80030E70(turn, 360.0 - angles.f.x * (180.0 / 3.141592654), 1.0f, 0.0f, 0.0f);
    func_8002CBC0(rotation, turn, rotation);
    func_8002CD40(rotation, &view);

    for (end = D_801D40CC + D_8016DB6C, rec = D_801D40CC; rec < end; rec++) {
        Model *model = rec->model_1C;
        s16 *max = model->max;
        s16 *min = model->min;
        u32 flags;
        f32 cx;
        f32 cy;
        f32 cz;

        if (!rec->used_24) {
            rec->depth_00 = 0.0f;
            continue;
        }
        cx = (max[0] + min[0]) / 2;
        cy = (max[1] + min[1]) / 2;
        cz = (max[2] + min[2]) / 2;
        func_8002CCC0(&D_801D9350[rec->matrix_20], &view, &world);
        func_8002CFB0(&world, cx, cy, cz, &x, &y, &rec->depth_00);
        flags = rec->model_1C->flags_28;
        if (flags & 0x20) {
            rec->class_27 = 0;
        } else if ((flags & 0x4C00) == 0xC00) {
            rec->class_27 = 1;
        } else if ((flags & 0x4C00) == 0x4C00) {
            rec->class_27 = 2;
        } else {
            rec->class_27 = 3;
        }
    }

    list = D_801D934C;
    lo = 0;
    hi = D_8016DB6C - 2;
    for (;;) {
        last = lo;
        for (j = lo; j <= hi; j++) {
            Slot *a = list[j];
            Slot *b = list[j + 1];
            if (b->used_24) {
                swap = 0;
                if (!a->used_24) {
                    swap = 1;
                } else if (a->class_27 > b->class_27) {
                    swap = 1;
                } else if (a->class_27 == b->class_27) {
                    if (a->model_1C->priority_18 > b->model_1C->priority_18) {
                        swap = 1;
                    } else if (a->model_1C->priority_18 == b->model_1C->priority_18) {
                        if (a->class_27 == 0) {
                            if (a->depth_00 < b->depth_00) {
                                swap = 1;
                            }
                        } else if (a->depth_00 > b->depth_00) {
                            swap = 1;
                        }
                    }
                }
                if (swap) {
                    last = j;
                    list[j] = b;
                    list[j + 1] = a;
                }
            }
        }
        if (last == lo) {
            break;
        }
        hi = --last;
        for (j = hi; j >= lo; j--) {
            Slot *a = list[j];
            Slot *b = list[j + 1];
            if (b->used_24) {
                swap = 0;
                if (!a->used_24) {
                    swap = 1;
                } else if (a->class_27 > b->class_27) {
                    swap = 1;
                } else if (a->class_27 == b->class_27) {
                    if (a->model_1C->priority_18 > b->model_1C->priority_18) {
                        swap = 1;
                    } else if (a->model_1C->priority_18 == b->model_1C->priority_18) {
                        if (a->class_27 == 0) {
                            if (a->depth_00 < b->depth_00) {
                                swap = 1;
                            }
                        } else if (a->depth_00 > b->depth_00) {
                            swap = 1;
                        }
                    }
                }
                if (swap) {
                    last = j;
                    list[j] = b;
                    list[j + 1] = a;
                }
            }
        }
        if (last == hi) {
            break;
        }
        lo = last + 1;
    }
}

void func_8006599C(void)
{
    func_80065560();
}

void func_800659B8(void)
{
    Vector camera;
    Mtx scale;
    float view[4][4], combined[4][4], temporary[4][4];
    s32 cell_x, cell_y;
    Mtx *world = &D_8016DB80[D_8016DB78];
    s32 translucent_color, opaque_color;
    s32 left = 0, top = 0, right = 0, bottom = 0;
    Cell *cell;
    s32 region = -1, use_variants = 0;
    Slot **entry_ptr, **entry_end;
    StateRecord *record, *record_end;
    s32 mode;
    float xmin, xmax, ymin, ymax;
    s32 in_front;
    u32 corner;
    /* Screen-space (NDC) bounds a projected box must overlap to be drawn. */
    float clip_left = -0.925f, clip_right = 0.925f, clip_bottom = -0.9166667f, clip_top = 0.9166667f;

    translucent_color = opaque_color = 0;
    if (D_8016DC0C != 0) return;

    /* Pipe sync and segment 5/6 bases for both the opaque and translucent lists. */
    { Gfx *g = D_801DEAB0++; g->w0 = 0xE7000000; g->w1 = 0; }
    { Gfx *g = D_801DEAB0++; g->w0 = 0xDB060014; g->w1 = func_800340F0(D_801DFF7C); }
    { Gfx *g = D_801DEAB0++; g->w0 = 0xDB060018; g->w1 = func_800340F0(D_801D2550); }
    { Gfx *g = D_801D2C24++; g->w0 = 0xE7000000; g->w1 = 0; }
    { Gfx *g = D_801D2C24++; g->w0 = 0xDB060014; g->w1 = func_800340F0(D_801DFF7C); }
    { Gfx *g = D_801D2C24++; g->w0 = 0xDB060018; g->w1 = func_800340F0(D_801D2550); }
    D_801DEAB0 = func_80062E88(D_801DEAB0);
    D_801D2C24 = func_80062E88(D_801D2C24);
    if (D_8013B984->field_04) {
        D_801DEAB0 = func_80069F14(D_801DEAB0);
        D_801D2C24 = func_80069F14(D_801D2C24);
    }
    func_80033A20(world, D_8013B984->field_0C, D_8013B984->field_10, D_8013B984->field_14);
    if (func_800627C4() == 1 && D_8016DC00 == 0x11) {
        func_80031220(&scale, 0.2f, 0.2f, 0.2f);
        func_8002CCC0(&scale, world, world);
    }
    /* gSPMatrix load of the world matrix (physical address). */
    { Gfx *g = D_801DEAB0++; g->w0 = 0xDA380003; g->w1 = (u32)world + 0x80000000U; }
    { Gfx *g = D_801D2C24++; g->w0 = 0xDA380003; g->w1 = (u32)world + 0x80000000U; }
    switch (D_8013B988) {
    case 2:
        func_80059B18(view);
        func_8002CE80(temporary, world);
        func_8002CBC0(temporary, view, view);
        break;
    case 3:
        func_80059B18(view);
        func_8002CE80(temporary, world);
        func_8002CBC0(temporary, view, combined);
        break;
    case 1:
        func_800593F8(&camera);
        left = (camera.x - 300.0f) - 320.0f;
        top = (camera.z - 440.0f) - 320.0f;
        right = (camera.x + 300.0f) - 320.0f;
        bottom = (camera.z + 300.0f) - 320.0f;
        break;
    }
    func_80063FA0();
    record = D_801D2564;
    record_end = record + D_8016DC08;
    while (record < record_end) {
        func_80063FB0(record->model, &record->state);
        record++;
    }
    mode = func_800627C4();
    if (mode == 1 && func_800627F4() != mode) {
        func_800418CC(&cell_x, &cell_y);
        region = func_80041574(cell_x, cell_y);
    }
    if (func_800627C4() == 1 && D_8016DC00 - 0x10 < 2) use_variants = 1;
    entry_ptr = D_801D934C;
    entry_end = entry_ptr + D_8016DB6C;
    for (; entry_ptr < entry_end; entry_ptr++) {
        Slot *entry = *entry_ptr;
        float *bounds = entry->bounds.v;
        Gfx **list;
        Gfx *gfx;
        s32 *color_active;
        State *state;
        u32 variant;

        if (!entry->used_24 || !(entry->visible_25 & 1)) continue;
        if (region >= 0) {
            cell_x = ((s32)(bounds[0] + bounds[1]) >> 6) + 10;
            if (cell_x < 10) cell_x = 10;
            else if (cell_x >= 66) cell_x = 65;
            cell_y = ((s32)(bounds[4] + bounds[5]) >> 6) + 10;
            if (cell_y < 10) cell_y = 10;
            else if (cell_y >= 44) cell_y = 43;
            if (func_80041574(cell_x, cell_y) != region) continue;
        }
        if (entry->class_27 == 3) {
            list = &D_801D2C24;
            color_active = &translucent_color;
        } else {
            list = &D_801DEAB0;
            color_active = &opaque_color;
        }
        gfx = *list;
        cell = &D_801DE97C->records[entry->matrix_20];
        if (entry->animation_26 != 0xFF) state = &D_801D2564[entry->animation_26].state;
        else state = 0;
        if (D_8013B988 == 3) {
            AnimationSet *set = entry->model_1C->animations_30;
            if (!set || !set->transform || !state->matrix) {
                ymin = 2.0f; ymax = -2.0f; xmin = 2.0f; xmax = -2.0f;
                in_front = 0;
                corner = 0;
                do {
                    float x = bounds[corner & 1];
                    float y = bounds[2 + ((corner >> 1) & 1)];
                    float z = bounds[4 + ((corner >> 2) & 1)];
                    float w = combined[0][3] * x + combined[1][3] * y + combined[2][3] * z + combined[3][3];
                    float sx, sy;
                    if (w == 0.0f) w = 0.000001f;
                    if (w > 0.0f) {
                        in_front = 1;
                        sx = (combined[0][0] * x + combined[1][0] * y + combined[2][0] * z + combined[3][0]) / w;
                        sy = (combined[0][1] * x + combined[1][1] * y + combined[2][1] * z + combined[3][1]) / w;
                    } else {
                        float tx, ty;
                        sx = combined[0][0] * x + combined[1][0] * y + combined[2][0] * z + combined[3][0];
                        tx = sx > 0.0f ? 2.0f : -2.0f;
                        sy = combined[0][1] * x + combined[1][1] * y + combined[2][1] * z + combined[3][1];
                        ty = sy > 0.0f ? 2.0f : -2.0f;
                        sx = tx;
                        sy = ty;
                    }
                    if (sx < xmin) xmin = sx;
                    if (xmax < sx) xmax = sx;
                    if (sy < ymin) ymin = sy;
                    if (ymax < sy) ymax = sy;
                } while (++corner < 8);
                if (!in_front || xmax < clip_left || xmin > clip_right || ymax < clip_bottom || ymin > clip_top) continue;
            }
        } else if (D_8013B988 == 2) {
            short *hi = entry->model_1C->max;
            short *lo = entry->model_1C->min;
            func_8002CE80(temporary, &D_801D9350[entry->matrix_20]);
            func_8002CBC0(temporary, view, combined);
            if (entry->model_1C->animations_30 && entry->model_1C->animations_30->transform && state->matrix) {
                func_8002CE80(temporary, state->matrix);
                func_8002CBC0(temporary, combined, combined);
            }
            ymin = 2.0f; ymax = -2.0f; xmin = 2.0f; xmax = -2.0f;
            in_front = 0;
            corner = 0;
            do {
                float x = (corner & 1) ? lo[0] : hi[0];
                float y = (corner & 2) ? lo[1] : hi[1];
                float z = (corner & 4) ? lo[2] : hi[2];
                float w = combined[0][3] * x + combined[1][3] * y + combined[2][3] * z + combined[3][3];
                float sx, sy;
                if (w == 0.0f) w = 0.000001f;
                if (w > 0.0f) {
                    in_front = 1;
                    sx = (combined[0][0] * x + combined[1][0] * y + combined[2][0] * z + combined[3][0]) / w;
                    sy = (combined[0][1] * x + combined[1][1] * y + combined[2][1] * z + combined[3][1]) / w;
                } else {
                    float tx, ty;
                    sx = combined[0][0] * x + combined[1][0] * y + combined[2][0] * z + combined[3][0];
                    tx = sx > 0.0f ? 2.0f : -2.0f;
                    sy = combined[0][1] * x + combined[1][1] * y + combined[2][1] * z + combined[3][1];
                    ty = sy > 0.0f ? 2.0f : -2.0f;
                    sx = tx;
                    sy = ty;
                }
                if (sx < xmin) xmin = sx;
                if (xmax < sx) xmax = sx;
                if (sy < ymin) ymin = sy;
                if (ymax < sy) ymax = sy;
            } while (++corner < 8);
            if (!in_front || xmax < clip_left || xmin > clip_right || ymax < clip_bottom || ymin > clip_top) continue;
        } else if (D_8013B988 == 1 && (bounds[0] != 0.0f || bounds[1] != 0.0f || bounds[4] != 0.0f || bounds[5] != 0.0f)) {
            if (bounds[1] < left || right < bounds[0] || bounds[5] < top || bottom < bounds[4]) continue;
        }
        { Gfx *g = gfx++; g->w0 = 0xDA380000; g->w1 = (u32)&D_801D9350[entry->matrix_20] + 0x80000000U; }
        if (!D_801DEAAC || (cell->flags & 0x8000)) {
            if (*color_active) {
                Gfx *g = gfx++;
                *color_active = 0;
                g->w0 = 0xFB000000; g->w1 = 0xFFFFFF00;
            }
        } else if (!*color_active) {
            Gfx *g = gfx++;
            *color_active = 1;
            g->w0 = 0xFB000000;
            g->w1 = ((u8)D_801DE9AC << 24) | ((u8)D_801E4E78 << 16) | ((u8)D_801DE9B0 << 8) | (u8)D_801DEAAC;
        }
        variant = (cell->flags >> 12) - 1;
        if (use_variants && variant < 7)
            gfx = func_80062EA4(gfx, entry->model_1C, state, 0, 0, 0, func_8004252C(variant));
        else
            gfx = func_80062EA4(gfx, entry->model_1C, state, 0, 0, 0, -1);
        { Gfx *g = gfx++; g->w0 = 0xD8380002; g->w1 = 0x40; }
        *list = gfx;
    }
    D_8016DB78 ^= 1;
}

s32 func_80066930(s32 value)
{
    s32 previous = D_8013B988;
    D_8013B988 = value;
    return previous;
}

s32 func_80066944(void)
{
    return D_8013B988;
}

void func_80066950(s32 *a, s32 *b, s32 *c, s32 *d) {
    if (D_8013B980 != 0) {
        if (a != 0) {
            *a = D_8013B980->settings->field_02;
        }
        if (b != 0) {
            *b = D_8013B980->settings->field_03;
        }
        if (c != 0) {
            *c = D_8013B980->settings->field_04;
        }
        if (d != 0) {
            *d = D_8013B980->settings->field_05;
        }
    } else {
        if (a != 0) {
            *a = 0;
        }
        if (b != 0) {
            *b = 0;
        }
        if (c != 0) {
            *c = 0x37;
        }
        if (d != 0) {
            *d = 0x21;
        }
    }
}

s32 func_800669FC(s32 i) {
    MenuState *t = D_8013B980;
    s32 r = 0;
    if (t != 0) {
        if (i >= 0) {
            r = t->settings->class_values_06[i];
        }
    }
    return r;
}

void func_80066A28(u32 *entryIndex, u32 *subIndex) {
    u32 count;
    u32 index;

    if (*entryIndex >= 16) {
        *entryIndex = 0;
    }
    index = *entryIndex;
    D_8016DC00 = index;
    D_8013B984 = &D_8013C084[index];
    func_8006AAF0(&count, D_8013B984->data_18, 4);
    if (*subIndex >= count) {
        *subIndex = 0;
    }
    D_8013B804 = func_80064A70(*subIndex);
    D_8013B800 = 1;
}

s32 func_80066AD0(s32 index) {
    s32 result;
    if (index >= 16) index = 0;
    func_8006AAF0(&result, D_8013C09C[index].romAddr, 4);
    return result;
}

MenuState *func_80066B20(void)
{
    return D_8013B980;
}
