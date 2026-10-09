#include "common.h"
typedef unsigned char u8;
typedef struct { u32 command, argument; } Gfx;
typedef struct { s32 start, count; u32 triangle_count; u8 (*triangles)[3]; } Batch;
typedef struct { s32 count; void *vertices; u32 batch_count; Batch *batches; } Mesh;
/* The renderer consumes the whole +0x48 subobject; its +0x1C member (object +0x64) is the
 * loaded texture-image pointer that func_80090588 copies from the TXIM resource's data. */
typedef struct { char fields_0[0x1C]; void *image_1C; } RenderEffect;
typedef struct { char fields_0[0x38]; Mesh mesh; RenderEffect effect_48; } Object;
extern void *func_800705F4(s32);
extern void *func_80032D94(void *destination, const void *source, unsigned int count);
extern Gfx *func_8006F994(Gfx *, void *);
static inline u32 vertex_opcode(s32 count) {
    return ((count & 127) << 1) | 0x01000000;
}
static inline u32 vertex_count(s32 count) { return (count & 255) << 12; }
/* render_state: the dispatcher supplies its local render-state pointer; this renderer does not read it. */
Gfx *func_8006F7F8(Gfx *output, void *render_state, Object *object) {
    Mesh *mesh = &object->mesh;
    char *vertices = func_800705F4(object->mesh.count);
    u32 i, j;
    if (vertices) {
        func_80032D94(vertices, mesh->vertices, (unsigned int)(object->mesh.count * 16));
        if (object->effect_48.image_1C) output = func_8006F994(output, &object->effect_48);
        for (i = 0; i < mesh->batch_count; i++) {
            Batch *batch = &mesh->batches[i];
            Gfx *command = output++;
            u32 count_bits = vertex_count(batch->count);
            command->command = count_bits | vertex_opcode(batch->count);
            command->argument = (u32)(vertices + batch->start * 16);
            for (j = 0; j < batch->triangle_count; j++) {
                command = output++;
                command->command = (((batch->triangles[j][0] * 2) & 255) << 16) |
                    ((batch->triangles[j][1] << 9) & 0xFE00) | ((batch->triangles[j][2] * 2) & 255) | 0x05000000;
                command->argument = 0;
            }
        }
    }
    return output;
}
