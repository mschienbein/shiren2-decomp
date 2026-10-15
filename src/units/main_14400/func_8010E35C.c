#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef short s16;
/* Kind 8 constructor 8010E290 installs D_8015D348; its action slot extends
 * the shared item-method prefix. */
typedef struct {
    ItemMethods base;
    s16 delta40;
    s16 index42;
    void (*apply44)(void *, void *);
} ItemActionMethods;
typedef struct { char pad[0x1E]; u8 flags1E; } Entity;
typedef struct { char pad[0xA]; u8 kindA; } Victim;
typedef struct { s32 x, y; } Point;
typedef struct { s32 id; void *arg4; Victim *arg8; s32 padC; Point position10; } Msg;
/* Damage fields written by func_80136910; natural target extent is 0x14. */
typedef struct {
    void *source;
    u32 kind;
    u32 field_8;
    unsigned short amount;
    unsigned short flags;
    u8 field_10;
} Path;
extern u32 D_8013960C;
s32 func_800E9270(Entity *, Item *, s32, s32);
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(void *obj);
char *func_800AC990(void *obj);
void func_800497F0(s32 message_id, ...);
void func_80049C90(s32, s32);
void func_80136910(void *obj, void *a, u32 c, u32 b, u32 e);
void func_800A7B68(Victim *, Path *);
void func_800D3650(Item *);
u32 func_800B1C6C(void *pos);
void func_8010E2CC(Item *);
s32 func_800AF28C(Item *, Msg *);
static inline void Path_init(Path *path, void *target, u32 a, u32 b) {
    func_80136910(path, target, a, b, 0);
}
static inline s32 Msg_id(Msg *msg) {
    return msg->id;
}
s32 func_8010E35C(Item *self, Msg *msg) {
    switch (Msg_id(msg)) {
    case 3: {
        Entity *entity = msg->arg4;
        if (((entity->flags1E >> 2) & 1) && func_800E9270(entity, self, 0x17, 0)) {
            ((ItemActionMethods *)self->field08)->apply44((char *)self + ((ItemActionMethods *)self->field08)->delta40, entity);
            if (self != 0) {
                self->field08->method0C((char *)self + self->field08->delta08, 3);
            }
            return 1;
        }
        return 0;
    }
    case 0x12: {
        Victim *item = msg->arg8;
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
                    self->field08->method0C((char *)self + self->field08->delta08, 3);
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
