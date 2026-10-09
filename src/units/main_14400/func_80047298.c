#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

/* g++ 2.x pointer-to-member descriptor; D_8014A9A8 = {0, 6, {.delta2 = 0x4C}} selects
 * virtual slot 6 through the vtable pointer at +0x4C. */
typedef struct {
    signed short delta, index;
    union { void (*pfn)(void *); signed short delta2; } u;
} MemberFn;
typedef struct {
    s32 type;
    s32 height;
    s32 x;
    s32 y;
} Rect;
typedef struct {
    u8 pad0[0x28];
    s32 x;
    s32 y;
    s16 height;
    u8 pad32[0x2CA];
    s32 mode;
    u8 pad300[0xE8];
    u8 unk3E8[0x10];
    u8 unk3F8[0x10];
    u16 visible;
} Window;
extern const MemberFn D_8014A9A8;
/* Rect records at .data 0x80138DE0 (record 1 at 0x80138DF0 is this window's frame). */
extern Rect D_80138DE0[];
void func_80095E58(void *dst, Window *object, MemberFn callback);
u8 *func_8006A810(void *dst, s32 value, s32 size);
void func_800486A4(void *, Rect *, void *);
void func_80047298(Window *self) {
    Rect rect;
    Rect tmp;

    func_80095E58(self->unk3F8, self, D_8014A9A8);
    switch (self->mode) {
    case 0x20:
        D_80138DE0[1].x = self->x;
        D_80138DE0[1].y = self->y - 8;
        func_800486A4(self->unk3E8, &D_80138DE0[1], self->unk3F8);
        break;
    case 0x40:
    case 0x80:
    case 0x100:
    case 0x200:
    case 0x1000:
        if (self->visible == 0) {
            break;
        }
        func_8006A810(&tmp, 0, sizeof(tmp));
        tmp.type = 2;
        tmp.height = self->height - 4;
        tmp.x = self->x - 3;
        tmp.y = self->y + 1;
        rect = tmp;
        func_800486A4(self->unk3E8, &rect, self->unk3F8);
        break;
    case 0x4:
    case 0x8:
    case 0x2000:
    case 0x4000:
        func_8006A810(&tmp, 0, sizeof(tmp));
        tmp.type = 2;
        tmp.height = self->height - 4;
        tmp.x = self->x - 3;
        tmp.y = self->y + 2;
        rect = tmp;
        func_800486A4(self->unk3E8, &rect, self->unk3F8);
        break;
    case 0x10:
        func_8006A810(&tmp, 0, sizeof(tmp));
        tmp.type = 2;
        tmp.height = self->height - 2;
        tmp.x = self->x - 3;
        tmp.y = self->y + 1;
        rect = tmp;
        func_800486A4(self->unk3E8, &rect, self->unk3F8);
        break;
    case 0x400:
        func_8006A810(&tmp, 0, sizeof(tmp));
        tmp.type = 2;
        tmp.height = self->height - 8;
        tmp.x = self->x - 3;
        tmp.y = self->y + 1;
        rect = tmp;
        func_800486A4(self->unk3E8, &rect, self->unk3F8);
        break;
    }
}
