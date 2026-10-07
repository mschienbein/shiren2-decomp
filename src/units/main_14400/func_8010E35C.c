#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct Actor Actor;
typedef struct {
    char pad0[8];
    s16 offset8;
    char padA[2];
    void (*destroyC)(void *, s32);
    char pad10[0x30];
    s16 offset40;
    char pad42[2];
    void (*func44)(void *, void *);
} ActorVTable;
struct Actor { s32 unk0; s32 unk4; ActorVTable *vtbl8; };
typedef struct { char pad[0x1E]; u8 flags1E; } Entity;
typedef struct { char pad[0xA]; u8 kindA; } Item;
typedef struct { s32 x, y; } Point;
typedef struct { s32 id; void *arg4; Item *arg8; s32 padC; Point position10; } Msg;
typedef struct { s32 data[6]; } Path;
extern u32 D_8013960C;
s32 func_800E9270(Entity *, Actor *, s32, s32);
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(void *obj);
char *func_800AC990(void *obj);
void func_800497F0(s32 message_id, ...);
void func_80049C90(s32, s32);
void func_80136910(void *obj, void *a, u32 c, u32 b, u32 e);
void func_800A7B68(Item *, Path *);
void func_800D3650(Actor *);
u32 func_800B1C6C(void *pos);
void func_8010E2CC(Actor *);
s32 func_800AF28C(Actor *, Msg *);
static inline void Path_init(Path *path, void *target, u32 a, u32 b) {
    func_80136910(path, target, a, b, 0);
}
static inline s32 Msg_id(Msg *msg) {
    return msg->id;
}
s32 func_8010E35C(Actor *self, Msg *msg) {
    switch (Msg_id(msg)) {
    case 3: {
        Entity *entity = msg->arg4;
        if (((entity->flags1E >> 2) & 1) && func_800E9270(entity, self, 0x17, 0)) {
            self->vtbl8->func44((char *)self + self->vtbl8->offset40, entity);
            if (self != 0) {
                self->vtbl8->destroyC((char *)self + self->vtbl8->offset8, 3);
            }
            return 1;
        }
        return 0;
    }
    case 0x12: {
        Item *item = msg->arg8;
        if (item->kindA == 0x23) {
            Path path;
            Path *route = &path;
            s32 text = func_80049CB4(0x21, item, 1, 0);
            func_800497F0(0x11D, text, func_800A3B20(item), func_800AC990(self));
            func_80049C90(0, text);
            Path_init(route, msg->arg4, 0, 6);
            D_8013960C <<= 1;
            func_800A7B68(item, route);
            D_8013960C >>= 1;
            if (msg->id == 0x12) {
                func_800D3650(self);
                if (self != 0) {
                    self->vtbl8->destroyC((char *)self + self->vtbl8->offset8, 3);
                }
            }
            return 1;
        }
        break;
    }
    case 0x1A:
        if (func_800B1C6C(&msg->position10) & 0x2000) {
            func_8010E2CC(self);
        }
        return 1;
    }
    return func_800AF28C(self, msg);
}
