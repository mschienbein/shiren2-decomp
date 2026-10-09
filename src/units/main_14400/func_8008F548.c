#include "common.h"
typedef float Matrix[4][4];
typedef struct { u32 count0; void *vertices; u32 count1; void *parts; } Geometry;
typedef struct { unsigned char pad00[0x18]; Geometry geometry; } Descriptor;
typedef struct {
    unsigned char type; unsigned char pad01[0x2B]; void *modelMatrix, *rotationMatrix;
    unsigned char pad34[4]; Geometry geometry; unsigned char pad48[0xF4]; Matrix transform;
    float x, y, z, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ;
} RenderNode;
extern void func_800312E0(Matrix matrix, float x, float y, float z);
extern void func_80030E70(Matrix matrix, float angle, float x, float y, float z);
extern void func_8002CBC0(Matrix left, Matrix right, Matrix result);
extern void func_80033B00(Matrix matrix, float x, float y, float z);
extern void *func_80070660(u32 count);
extern void func_8002CD40(Matrix matrix, void *fixedMatrix);
/* Descriptor callback at +0xC receives descriptor, actor, and render node. */
s32 func_8008F548(Descriptor *descriptor, void *actor, RenderNode *node) {
    /* The dispatcher supplies actor, unused by geometry application. */
    Matrix translation, rotation, temporary;
    s32 result = 0;
    Geometry *geometry;
    /* ODD_C: single-pass block skipping matrix conversion and geometry assignment when a
     * matrix allocation fails; it also shapes the prologue scheduling. */
    do {
        func_800312E0(rotation, node->scaleX, node->scaleY, node->scaleZ);
        func_80030E70(temporary, node->rotationX, 1.0f, 0.0f, 0.0f);
        func_8002CBC0(rotation, temporary, rotation);
        func_80030E70(temporary, node->rotationY, 0.0f, 1.0f, 0.0f);
        func_8002CBC0(rotation, temporary, rotation);
        func_80030E70(temporary, node->rotationZ, 0.0f, 0.0f, 1.0f);
        func_8002CBC0(rotation, temporary, rotation);
        func_80033B00(translation, node->x, node->y, node->z);
        func_8002CBC0(translation, node->transform, translation);
        if ((node->modelMatrix = func_80070660(1)) == 0 || (node->rotationMatrix = func_80070660(1)) == 0) {
            result = -1;
            break;
        }
        func_8002CD40(translation, node->modelMatrix);
        func_8002CD40(rotation, node->rotationMatrix);
        geometry = &node->geometry;
        if (node->type == 0) {
            geometry->count0 = descriptor->geometry.count0;
            geometry->vertices = descriptor->geometry.vertices;
            geometry->count1 = descriptor->geometry.count1;
            geometry->parts = descriptor->geometry.parts;
        }
    } while (0);
    return result;
}
