#include "common.h"

typedef unsigned char u8;

typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f800865A0;

typedef struct {
    u8 pad0[4];
    u16 result4;
    u8 pad6[2];
    u16 state8;
    u16 counterA;
    u8 padC[2];
    u16 doneE;
    u8 pad10[2];
    u16 flags12;
    s32 handle14;
    u8 pad18[4];
    s32 timer1C;
    u8 pad20[8];
    s32 sound28;
    s32 model2C;
    f32 endFrame30;
    f32 speed34;
    u8 pad38[4];
    f32 dx3C;
    f32 x40;
    u8 pad44[4];
    f32 dz48;
    f32 z4C;
    u8 pad50[0xC];
    s32 tileX5C;
    u8 pad60[4];
    s32 anim64;
    s32 tileZ68;
    u8 pad6C[4];
    s32 arg70;
} Obj800865A0;

extern s32 func_8008BF14(s32 model, s32 anim, s32 arg2, s32 loop, s32 arg4, f32 *delta, f32 scale);
extern void func_800892B0(Obj800865A0 *obj, f32 speed);
extern void func_8008C334(s32 handle);
extern void func_8008C478(s32 handle, u8 on);
extern s32 func_80084014(s32 tileX, s32 tileZ);
extern void func_8008C588(s32 handle, Vec3f800865A0 *pos, f32 scale);
extern s32 func_8008C1C8(s32 handle, f32 step);
extern f32 func_8008C440(s32 handle);
extern void func_80052260(s16 sound);
extern void func_8008C194(s32 handle);

void func_800865A0(Obj800865A0 *obj) {
    Vec3f800865A0 pos;
    s32 started = 0;
    s32 status;
    s32 tileX;
    s32 tileZ;
    f32 speed;
    f32 centerX;
    f32 centerZ;

    switch (obj->state8) {
        case 0:
            started = 1;
            speed = obj->speed34;
            obj->handle14 = func_8008BF14(obj->model2C, obj->anim64, obj->arg70, !((obj->flags12 >> 5) & 1), 0,
                                          &obj->dx3C, 1.0f);
            if (obj->handle14 < 0) {
                obj->result4 = 4;
                return;
            }
            func_800892B0(obj, speed);
            obj->dx3C *= 0.25f;
            obj->dz48 *= 0.25f;
            if (obj->flags12 & 0x1000) {
                obj->x40 -= obj->dx3C;
                obj->z4C -= obj->dz48;
            }
            func_8008C334(obj->handle14);
            obj->counterA = 0;
            obj->state8++;
            /* fallthrough */
        case 1:
            if (obj->timer1C == 0) {
                obj->doneE = 1;
                func_8008C478(obj->handle14, 0);
                obj->timer1C = 1;
                obj->state8++;
                return;
            }
            do {
                obj->timer1C--;
                obj->x40 -= obj->dx3C;
                obj->z4C -= obj->dz48;
                centerX = obj->tileX5C * 32 + 16;
                centerZ = obj->tileZ68 * 32 + 16;
                pos.x = centerX;
                pos.y = 0.0f;
                pos.z = centerZ;
                pos.x += (s32)obj->x40;
                pos.z += (s32)obj->z4C;
                tileX = pos.x / 32.0f;
                tileZ = pos.z / 32.0f;
                if (!(obj->flags12 & 0x100) || started || obj->timer1C == 0) {
                    break;
                }
            } while (func_80084014(tileX, tileZ) == 0);
            func_8008C588(obj->handle14, &pos, 1.0f);
            status = func_8008C1C8(obj->handle14, 1.0f);
            if (func_8008C440(obj->handle14) == obj->endFrame30) {
                if (++obj->counterA == 1 && obj->sound28 >= 0) {
                    func_80052260(obj->sound28);
                }
            }
            if (status & 2) {
                obj->doneE = 1;
            }
            if (status & 1) {
                func_8008C334(obj->handle14);
            }
            break;
        case 2:
            if (obj->timer1C-- == 0) {
                func_8008C194(obj->handle14);
                obj->result4 = 4;
            }
            break;
    }
}
