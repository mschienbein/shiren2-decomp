#include "common.h"
typedef union { u32 word; struct { unsigned short tag, duration; } fields; } FrameHeader;
typedef struct { FrameHeader header; unsigned short field_4, field_6, field_8, field_A; } TextureFrame;
typedef struct { FrameHeader header; float x, y; } UVFrame;
typedef struct { FrameHeader header; float tx, ty, tz, rx, ry, rz, sx, sy, sz; } TransformFrame;
typedef struct { s32 field_0; TextureFrame *texture; UVFrame *uv; TransformFrame *transform; } Animation;
typedef struct { char pad[0x30]; Animation *field_30; } Obj;
typedef float Matrix[4][4];
typedef struct {
    unsigned short field_0, field_2; float u, v;
    float tx, ty, tz, rx, ry, rz, sx, sy, sz;
    short textureTime; TextureFrame *texture;
    short uvTime; UVFrame *uv;
    short transformTime; TransformFrame *transform;
    Matrix *matrix;
} State;
extern s32 D_8013B808;
extern u32 D_8016CB14;
extern Matrix D_8016CB18[2][32];
extern void func_80030FD0(Matrix *, float, float, float, float), func_8002CCC0(Matrix *, Matrix *, Matrix *), func_80031220(Matrix *, float, float, float), func_80033A20(Matrix *, float, float, float);
void func_80063FB0(Obj *a, State *b) {
    Matrix first, second;
    if (a->field_30 && b) {
        if (a->field_30->texture) {
            if (b->textureTime > 0) b->textureTime--;
            else {
                if (!b->texture) b->texture = a->field_30->texture;
                else { b->texture++; if (!b->texture->header.word) b->texture = a->field_30->texture; }
                b->textureTime = b->texture->header.fields.duration - 1;
                b->field_0 = b->texture->field_A;
                b->field_2 = b->texture->field_6;
            }
        }
        if (a->field_30->uv) {
            if (b->uvTime > 0) b->uvTime--;
            else {
                if (!b->uv || (++b->uv, !b->uv->header.word)) {
                    b->u = 0.0f; b->v = 0.0f; b->uv = a->field_30->uv;
                }
                b->uvTime = b->uv->header.fields.duration - 1;
                if (!a->field_30->texture) { b->field_0 = 0; b->field_2 = 0; }
            }
            b->u += b->uv->x; b->v += b->uv->y;
        }
        if (a->field_30->transform) {
            if (b->transformTime > 0) b->transformTime--;
            else {
                if (!b->transform) {
                    b->transform = a->field_30->transform;
                    b->tx = 0.0f; b->ty = 0.0f; b->tz = 0.0f;
                    b->rx = 0.0f; b->ry = 0.0f; b->rz = 0.0f;
                    b->sx = 1.0f; b->sy = 1.0f; b->sz = 1.0f;
                } else {
                    b->transform++;
                    if (!b->transform->header.word) {
                        TransformFrame *reset = a->field_30->transform;
                        b->tx = 0.0f; b->ty = 0.0f; b->tz = 0.0f;
                        b->rx = 0.0f; b->ry = 0.0f; b->rz = 0.0f;
                        b->sx = 1.0f; b->sy = 1.0f; b->sz = 1.0f;
                        b->transform = reset;
                    }
                }
                b->transformTime = b->transform->header.fields.duration - 1;
            }
            b->tx += b->transform->tx; b->ty += b->transform->ty; b->tz += b->transform->tz;
            b->rx += b->transform->rx; b->ry += b->transform->ry; b->rz += b->transform->rz;
            b->sx += b->transform->sx; b->sy += b->transform->sy; b->sz += b->transform->sz;
            if (D_8016CB14 < 32) {
                Matrix *out = &D_8016CB18[D_8013B808][D_8016CB14];
                Matrix *temp;
                func_80030FD0(&first, b->rx, 1.0f, 0.0f, 0.0f);
                temp = &second;
                func_80030FD0(temp, b->ry, 0.0f, 1.0f, 0.0f);
                func_8002CCC0(&first, temp, &first);
                func_80030FD0(temp, b->rz, 0.0f, 0.0f, 1.0f);
                func_8002CCC0(&first, temp, &first);
                func_80031220(temp, b->sx, b->sy, b->sz);
                func_8002CCC0(temp, &first, out);
                func_80033A20(temp, b->tx, b->ty, b->tz);
                func_8002CCC0(out, temp, out);
                b->matrix = out;
            } else b->matrix = 0;
            D_8016CB14++;
        }
    }
}
