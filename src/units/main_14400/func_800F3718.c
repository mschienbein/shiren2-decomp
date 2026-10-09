#include "common.h"

typedef unsigned char u8;
typedef struct { short delta, index; void *(*call)(void *); } GetEntry;
typedef struct { char pad0[0x98]; GetEntry get_98; } Vtable;
typedef union { unsigned short value; struct { u8 high, low; } bytes; } Flags;
typedef struct { char pad0[8]; u8 direction_8; char pad9[0x15]; u8 flags_1E; char pad1F[5]; Vtable *vtbl_24; char pad28[0x70]; unsigned short message_98; Flags flags_9A; } Object;
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800E2074(void *obj);
extern void *func_800A65E4(void *out, void *obj, void *target);
extern void func_800A665C(void *obj, u8 *value);
extern char *func_800A3B20(void *obj);
extern void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32 mode);
extern s32 func_800E2044(void *obj);
extern u8 *func_800F0314(void *obj);
extern s32 func_800CD090(void *collection, void *element);
extern s32 func_80121940(void *item, void *obj, void *target, s32 mode);
extern void func_800A6690(void *obj, u8 *value, s32 count);

s32 func_800F3718(Object *obj, Object *target) {
    u8 state[2];
    func_80049CB4(0x128, 0x1A6);
    if (func_800E2074(obj)) {
        u8 *direction = state;
        state[1] = obj->direction_8;
        func_80049CB4(9);
        func_800A65E4(direction, obj, target);
        func_800A665C(obj, direction);
        if (obj->flags_9A.bytes.high & 1) {
            func_800498E4(0xAE, func_800A3B20(obj));
            func_80049BF0(1);
            obj->flags_9A.value &= ~0x100;
        } else {
            s32 attached;
            func_800498E4(obj->message_98);
            func_80049BF0(0);
            attached = ((target->flags_1E >> 2) & 1) &&
                (obj->flags_9A.bytes.low >> 7) && func_800E2044(obj);
            if (attached) {
                u8 *item = func_800F0314(obj);
                if (item) {
                    GetEntry *entry = &target->vtbl_24->get_98;
                    void *collection = entry->call((char *)target + entry->delta);
                    if (func_800CD090(collection, item) != -1) {
                        func_80121940(item, obj, target, 0);
                    }
                }
            } else {
                func_800A6690(obj, &state[1], 1);
            }
        }
    } else {
        func_800498E4(0x1BD, func_800A3B20(obj));
        func_80049BF0(0);
    }
    return 1;
}
