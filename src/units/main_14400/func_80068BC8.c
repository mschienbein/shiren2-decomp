#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef u32 size_t;
typedef struct { u32 w0, w1; } Gfx;
typedef struct { s32 m[4][4]; } Mtx;
typedef struct { float x, y, z; } Vector;
typedef struct { short x_00, y_02, z_04; u16 flag_06; short s_08, t_0A; u8 r_0C, g_0D, b_0E, a_0F; } Vertex;
typedef struct { u32 field_00; Vertex *vertices_04; u32 count_08; u8 pad_0C[0x1C]; u32 flags_28; } Model;
typedef struct { u8 active_00, tile_01, instance_02, pad_03; u16 id_04, pad_06; Model *model_08; Vertex *vertices_0C[2]; } SceneObject;
typedef struct { u8 active_00, count_01[2], pad_03[5]; Vertex vertices_08[2][32]; } SceneChunk;
typedef struct { u8 bytes[0x30]; } Obj;
typedef struct { u8 bytes[0x4C]; } Animation;
typedef struct { Obj *node; Animation animation; } ActiveNode;
typedef struct { u8 kind, subtype, flags; } Cell;
typedef struct { u8 field_00, fog_01, alpha_02; u8 pad_03[0x21]; } SceneEntry;

/* Convert to an integer only when encoding the RDP address word. */
#define OS_K0_TO_PHYSICAL(x) ((u32)(x) - 0x80000000U)

extern Gfx *D_801DEAB0, *D_801D2C24, *D_801D2C04;
extern void *D_801D85A8;
extern void *D_801D2C08;
extern s32 D_801D2C20;
extern s32 D_801E4E70;
extern s32 D_801E4E74;
extern u8 D_8016DC10;
extern u8 D_8016DC11, D_8016DC12;
extern SceneEntry D_8013C4FC[];
extern ActiveNode *D_801D2C00;
extern SceneObject *D_801D2560;
extern SceneChunk *D_801D40D4;
extern Cell *D_801E02A4;
extern Mtx D_8016DC18[2], D_8016DC98[2];

u32 func_800340F0(void *addr);
Gfx *func_80062E88(Gfx *g);
void func_8006A0AC(unsigned char *arg0, unsigned char *arg1, unsigned char *arg2, unsigned char *arg3);
void func_800593F8(Vector *out);
void func_80031220(Mtx *m, float x, float y, float z);
void func_80033A20(Mtx *m, float x, float y, float z);
void func_8002CCC0(Mtx *m, Mtx *n, Mtx *res);
float func_80062CAC(void);
void func_800679F0(u32 x, u32 z);
void func_80063FA0(void);
void func_80063FB0(Obj *a, Animation *b);
u32 func_800699C8(u32 value);
void *func_80032D94(void *s1, const void *s2, size_t n);
Gfx *func_80062EA4(Gfx *gfx, Model *model, Animation *animation, s32 count, Vertex *vertices, u8 alpha, s32 flags);
/* The callee stores a float through each non-null pointer (swc1 via a0..a3 and
 * both stack arguments), so all six parameters are float pointers. */
void func_80061AB8(float *arg0, float *arg1, float *minX, float *minZ, float *maxX, float *maxZ);
s32 func_8005980C(void);

void func_80068BC8(void)
{
    Vector camera;
    Mtx matrix;
    u8 red, green, blue, alpha;
    u8 shaded_opaque, shaded_translucent;
    float bound_a, bound_b, min_x, min_z, max_x, max_z;
    Gfx **opaque_list, **translucent_list, **special_list;
    s32 map_x, map_z;
    ActiveNode *node, *node_end;
    SceneObject *object;
    SceneChunk *chunk;
    Vertex *vertex;
    u32 i;
    shaded_opaque = 0;
    shaded_translucent = 0;

    opaque_list = &D_801DEAB0;
    { Gfx *g = (*opaque_list)++; g->w0 = 0xE7000000; g->w1 = 0; }
    { Gfx *g = (*opaque_list)++; g->w0 = 0xDB060014; g->w1 = func_800340F0(D_801D85A8); }
    { Gfx *g = (*opaque_list)++; g->w0 = 0xDB060018; g->w1 = func_800340F0(D_801D2C08); }
    translucent_list = &D_801D2C24;
    { Gfx *g = (*translucent_list)++; g->w0 = 0xE7000000; g->w1 = 0; }
    { Gfx *g = (*translucent_list)++; g->w0 = 0xDB060014; g->w1 = func_800340F0(D_801D85A8); }
    { Gfx *g = (*translucent_list)++; g->w0 = 0xDB060018; g->w1 = func_800340F0(D_801D2C08); }
    special_list = &D_801D2C04;
    { Gfx *g = (*special_list)++; g->w0 = 0xE7000000; g->w1 = 0; }
    { Gfx *g = (*special_list)++; g->w0 = 0xDB060014; g->w1 = func_800340F0(D_801D85A8); }
    { Gfx *g = (*special_list)++; g->w0 = 0xDB060018; g->w1 = func_800340F0(D_801D2C08); }

    D_801DEAB0 = func_80062E88(D_801DEAB0);
    D_801D2C24 = func_80062E88(D_801D2C24);
    D_801D2C04 = func_80062E88(D_801D2C04);

    func_8006A0AC(&red, &green, &blue, &alpha);
    {
        u32 color = (red << 24) | (green << 16) | (blue << 8) | alpha;
        { Gfx *g = (*opaque_list)++; g->w0 = 0xF8000000; g->w1 = color; }
        { Gfx *g = (*translucent_list)++; g->w0 = 0xF8000000; g->w1 = color; }
        { Gfx *g = (*special_list)++; g->w0 = 0xF8000000; g->w1 = color; }
    }
    { Gfx *g = (*opaque_list)++; g->w0 = 0xFB000000; g->w1 = 0xFFFFFF00; }
    { Gfx *g = (*translucent_list)++; g->w0 = 0xFB000000; g->w1 = 0xFFFFFF00; }
    { Gfx *g = (*special_list)++; g->w0 = 0xFB000000; g->w1 = D_8013C4FC[D_8016DC11].alpha_02; }

    func_800593F8(&camera);
    map_x = ((s32)camera.x >> 5) - 5;
    map_z = ((s32)camera.z >> 5) - 4;
    if (map_x <= 0) map_x = 1; else if (map_x >= 65) map_x = 64;
    if (map_z <= 0) map_z = 1; else if (map_z >= 45) map_z = 44;

    func_80031220(&matrix, 1.0f, 1.0f, 1.0f);
    func_80033A20(&D_8016DC18[D_8016DC12], 0.0f, 0.0f, 0.0f);
    func_8002CCC0(&matrix, &D_8016DC18[D_8016DC12], &D_8016DC18[D_8016DC12]);
    func_80031220(&D_8016DC98[D_8016DC12], 1.0f, func_80062CAC(), 1.0f);
    func_8002CCC0(&D_8016DC98[D_8016DC12], &D_8016DC18[D_8016DC12], &D_8016DC98[D_8016DC12]);
    func_800679F0(map_x, map_z);

    func_80063FA0();
    node = D_801D2C00;
    node_end = node + D_8016DC10;
    for (; node < node_end; node++) {
        func_80063FB0(node->node, &node->animation);
    }

    for (object = D_801D2560; object < D_801D2560 + 198; object++) {
        u32 kind = func_800699C8(object->id_04);
        if (object->active_00) {
            Gfx **list;
            Gfx *gfx;
            u8 *shaded = 0;
            u32 x, z;
            Animation *animation;
            u8 mode_alpha;

            if (kind - 6 < 2) {
                list = &D_801D2C04;
            } else if (object->model_08->flags_28 & 0x20) {
                list = &D_801DEAB0;
                shaded = &shaded_opaque;
            } else {
                list = &D_801D2C24;
                shaded = &shaded_translucent;
            }
            gfx = *list;
            if (kind == 2 || kind == 5 || kind == 6) {
                Gfx *g = gfx++;
                g->w0 = 0xDA380003;
                g->w1 = OS_K0_TO_PHYSICAL(&D_8016DC98[D_8016DC12]);
            } else {
                Gfx *g = gfx++;
                g->w0 = 0xDA380003;
                g->w1 = OS_K0_TO_PHYSICAL(&D_8016DC18[D_8016DC12]);
            }

            x = (u8)(object->tile_01 % 11);
            z = (u8)(object->tile_01 / 11);
            i = map_x % 11;
            x = map_x - i + x + (x < i ? 11 : 0);
            i = map_z % 9;
            z = map_z - i + z + (z < i ? 9 : 0);

            if (object->vertices_0C[D_8016DC12] == 0) {
                chunk = &D_801D40D4[object->tile_01];
                vertex = object->vertices_0C[D_8016DC12] = &chunk->vertices_08[D_8016DC12][chunk->count_01[D_8016DC12]];
                func_80032D94(vertex, object->model_08->vertices_04, object->model_08->count_08 * sizeof(Vertex));
                chunk->count_01[D_8016DC12] += object->model_08->count_08;
                for (i = 0; i < object->model_08->count_08; i++, vertex++) {
                    vertex->x_00 += (x << 5) + 16;
                    vertex->z_04 += (z << 5) + 16;
                }
            }

            if (shaded != 0) {
                if (D_801E02A4[z * 76 + x].flags & 2) {
                    if (*shaded == 0) {
                        *shaded = 1;
                        { Gfx *g = gfx++; g->w0 = 0xE7000000; g->w1 = 0; }
                        { Gfx *g = gfx++; g->w0 = 0xFB000000; g->w1 = 0x80806000; }
                    }
                } else if (*shaded != 0) {
                    *shaded = 0;
                    { Gfx *g = gfx++; g->w0 = 0xE7000000; g->w1 = 0; }
                    { Gfx *g = gfx++; g->w0 = 0xFB000000; g->w1 = 0xFFFFFF00; }
                }
            }

            if (object->instance_02 != 0xFF) {
                animation = &D_801D2C00[object->instance_02].animation;
            } else {
                animation = 0;
            }
            if (kind == 6) {
                mode_alpha = D_8013C4FC[D_8016DC11].fog_01;
            } else {
                mode_alpha = 0;
            }
            *list = func_80062EA4(gfx, object->model_08, animation, object->model_08->count_08,
                                  object->vertices_0C[D_8016DC12], mode_alpha, -1);
        }
    }

    if (D_801E4E70 != 0 || D_801D2C20 != 0) {
        s32 facing;
        func_80061AB8(&bound_a, &bound_b, &min_x, &min_z, &max_x, &max_z);
        facing = func_8005980C();
        for (chunk = D_801D40D4; chunk < D_801D40D4 + 99; chunk++) {
            vertex = chunk->vertices_08[D_8016DC12];
            for (i = 0; i < chunk->count_01[D_8016DC12]; i++) {
                float z, x;
                x = vertex[i].x_00;
                z = vertex[i].z_04;
                if (D_801E4E70 != 0) {
                    if (x < min_x - 75.0f || max_x + 75.0f < x || z < min_z - 75.0f || max_z + 75.0f < z) {
                        vertex[i].a_0F = D_801E4E74;
                    } else {
                        x = x < min_x ? min_x - x : (max_x < x ? x - max_x : 0.0f);
                        z = z < min_z ? min_z - z : (max_z < z ? z - max_z : 0.0f);
                        z = (__builtin_sqrtf(x * x + z * z) - 25.0f) * ((float)D_801E4E74 / 50.0f);
                        if (z <= 0.0f) {
                            vertex[i].a_0F = 0;
                        } else if ((float)D_801E4E74 <= z) {
                            vertex[i].a_0F = D_801E4E74;
                        } else {
                            vertex[i].a_0F = (u32)z;
                        }
                    }
                } else {
                    vertex[i].a_0F = 0;
                }
                if (D_801D2C20 != 0) {
                    x = (float)vertex[i].x_00 - camera.x;
                    z = (float)vertex[i].z_04 - camera.z;
                    if (x > 0.0f) {
                        if (facing == 0 || facing == 1 || facing == 7) x = 0.0f;
                        else x = x - 144.0f;
                    } else if (facing == 3 || facing == 4 || facing == 5) {
                        x = 0.0f;
                    } else {
                        x = -(x - -144.0f);
                    }
                    if (z > 0.0f) {
                        if (facing == 5 || facing == 6 || facing == 7) z = 0.0f;
                        else z = z - 112.0f;
                    } else if (facing == 1 || facing == 2 || facing == 3) {
                        z = 0.0f;
                    } else {
                        z = -(z - -112.0f);
                    }
                    if (z < x) z = x;
                    if (z > 0.0f) {
                        double fade = z;
                        if (fade >= 32.0) {
                            z = 256.0f;
                        } else if (vertex[i].a_0F == 0) {
                            z = fade * 8.0;
                        } else {
                            z = vertex[i].a_0F + (float)(256.0 - vertex[i].a_0F) * (fade * 0.03125);
                        }
                        if (z < 255.0f) vertex[i].a_0F = (u32)z;
                        else vertex[i].a_0F = 255;
                    }
                }
            }
        }
    } else {
        for (chunk = D_801D40D4; chunk < D_801D40D4 + 99; chunk++) {
            vertex = chunk->vertices_08[D_8016DC12];
            for (i = 0; i < chunk->count_01[D_8016DC12]; i++) {
                vertex[i].a_0F = 0;
            }
        }
    }
    D_8016DC12 ^= 1;
}
