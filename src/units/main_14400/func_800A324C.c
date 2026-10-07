#include "common.h"

typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} Rect;

Rect *func_800A324C(Rect *out, Rect *a, Rect *b) {
    Rect r;

    s32 top;
    s32 left;
    s32 bottom;
    s32 right;

    top = b->top;
    if (top < a->top) {
        top = a->top;
    }
    r.top = top;
    left = b->left;
    if (left < a->left) {
        left = a->left;
    }
    r.left = left;
    bottom = b->bottom;
    if (a->bottom < bottom) {
        bottom = a->bottom;
    }
    r.bottom = bottom;
    right = b->right;
    if (a->right < right) {
        right = a->right;
    }
    r.right = right;
    out->left = r.left;
    out->top = r.top;
    out->right = r.right;
    out->bottom = r.bottom;
    return out;
}
