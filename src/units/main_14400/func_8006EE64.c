#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct { f32 m[4][4]; } Mtx;

/* Draw object header: kind and flags followed by the sort state byte. */
typedef struct {
    u8 kind;
    u8 flags;    /* bit 0: visible, bit 4: billboard, bit 5: unsorted */
    u8 sort;      /* bit 0: culled, bits 1-3: layer */
    u8 pad3;
    u8 pad4[0xC];
    u32 render_10;
    u8 pad14[0x18];
    void *model_2C;
    u8 pad30[4];
    f32 depth_34;
    u8 pad38[4];
    u8 flags_3C;
    u8 pad3D[3];
    s32 priority_40;
} DrawObj;

typedef struct {
    u32 count;
    u8 pad4[4];
    DrawObj **items;
} DrawList;

void func_8002CE80(Mtx *m, void *model);
void func_80059B18(Mtx *dst);
void func_8002CBC0(float mf[4][4], float nf[4][4], float res[4][4]);

/* Classifies every draw object, cocktail-sorts the list and drops trailing hidden entries. */
void func_8006EE64(DrawList *list)
{
    Mtx model;
    Mtx view;
    s32 i;
    DrawObj *obj;
    s32 swap;
    u8 aLayer;
    u8 bLayer;
    DrawObj *a;
    DrawObj *b;

    for (i = 0; i < list->count; i++) {
        obj = list->items[i];
        obj->sort &= 0xE;
        if (!(obj->flags & 1)) {
            continue;
        }
        if (obj->kind < 2) {
            if (!(obj->kind == 1 && (obj->flags & 0x10))) {
                func_8002CE80(&model, obj->model_2C);
                func_80059B18(&view);
                func_8002CBC0(model.m, view.m, model.m);
                if (!(model.m[3][3] > 0.0f)) {
                    obj->sort |= 1;
                    continue;
                }
                obj->depth_34 = model.m[3][2];
            }
            if (!(obj->flags & 0x20)) {
                u32 mode;

                obj->sort &= 0xF1;
                mode = obj->render_10;
                if (mode & 0x20) {
                    obj->sort |= 2;
                } else if ((mode & 0x4C00) == 0xC00) {
                    obj->sort |= 4;
                } else if ((mode & 0x4C00) == 0x4C00) {
                    obj->sort |= 6;
                } else {
                    obj->sort |= 8;
                }
            }
        } else {
            obj->sort = 0;
            if (obj->flags_3C & 1) {
                obj->sort = 10;
            }
        }
    }

    {
        s32 start = 0;
        s32 end = list->count - 2;
        s32 last;

        for (;;) {
            last = start;
            for (i = start; i <= end; i++) {
                b = list->items[i + 1];
                a = list->items[i];

                if (!(b->flags & 1)) {
                    continue;
                }
                if (b->sort & 1) {
                    continue;
                }
                bLayer = (b->sort >> 1) & 7;
                aLayer = (a->sort >> 1) & 7;
                swap = 0;
                if (!(a->flags & 1) || (a->sort & 1) || aLayer > bLayer) {
                    swap = 1;
                } else if (aLayer == bLayer) {
                    if (aLayer == 0 || aLayer == 5) {
                        if (a->priority_40 > b->priority_40) {
                            swap = 1;
                        }
                    } else if (a->depth_34 < b->depth_34) {
                        swap = 1;
                    }
                }
                if (swap) {
                    list->items[i] = b;
                    list->items[i + 1] = a;
                    last = i;
                }
            }
            if (last == start) {
                break;
            }
            end = last - 1;
            last = end;
            for (i = end; i >= start; i--) {
                b = list->items[i + 1];
                a = list->items[i];

                if (!(b->flags & 1)) {
                    continue;
                }
                if (b->sort & 1) {
                    continue;
                }
                bLayer = (b->sort >> 1) & 7;
                aLayer = (a->sort >> 1) & 7;
                swap = 0;
                if (!(a->flags & 1) || (a->sort & 1) || aLayer > bLayer) {
                    swap = 1;
                } else if (aLayer == bLayer) {
                    if (aLayer == 0 || aLayer == 5) {
                        if (a->priority_40 > b->priority_40) {
                            swap = 1;
                        }
                    } else if (a->depth_34 < b->depth_34) {
                        swap = 1;
                    }
                }
                if (swap) {
                    list->items[i] = b;
                    list->items[i + 1] = a;
                    last = i;
                }
            }
            if (last == end) {
                break;
            }
            start = last + 1;
        }
    }

    while (list->count != 0) {
        obj = list->items[list->count - 1];
        if ((obj->flags & 1) && !(obj->sort & 1)) {
            break;
        }
        list->count--;
    }
}
