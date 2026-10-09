#include "common.h"
typedef struct {
    unsigned char pad_00[0x60]; short adjustment_60; unsigned short pad_62;
    s32 (*contains_64)(void *, void *, s32);
} ContainerVTable800DC524;
typedef struct { unsigned char pad_00[4]; ContainerVTable800DC524 *vtable_04; } Container800DC524;
typedef struct {
    s32 kind; void *sender; void *target; unsigned char pad_0C[0x14];
} Message800DC524;
typedef struct {
    unsigned char pad_00[0x38]; short adjustment_38; unsigned short pad_3A;
    s32 (*action_3C)(void *, Message800DC524 *);
} ItemVTable800DC524;
typedef struct { unsigned char pad_00[8]; ItemVTable800DC524 *vtable_08; } Item800DC524;
typedef struct { Container800DC524 *container; Item800DC524 *item; } Link800DC524;
typedef struct { unsigned char pad_00[8]; Link800DC524 first_08; Link800DC524 second_10; } Obj800DC524;
extern void *D_801476B8;
extern void func_800DAD20(void *self, void *item);
extern s32 func_800CD2BC(void *container, void *item);
extern void func_800DAD80(void *self);
s32 func_800DC524(Obj800DC524 *self) {
    Link800DC524 *first = &self->first_08;
    Link800DC524 *second;
    Item800DC524 *item;
    Item800DC524 *target;
    Message800DC524 message;
    Message800DC524 *command;
    s32 failed = first->container->vtable_04->contains_64((unsigned char *)first->container + first->container->vtable_04->adjustment_60, first->item, 1) != 1;
    if (failed) {
        return 0;
    }
    second = &self->second_10;
    item = first->item;
    func_800DAD20(self, second->item);
    func_800DAD20(self, item);
    target = second->item;
    message.kind = 2;
    command = &message;
    command->sender = D_801476B8;
    command->target = target;
    if (item->vtable_08->action_3C((unsigned char *)item + item->vtable_08->adjustment_38, command)) {
        func_800CD2BC(self->first_08.container, item);
    }
    func_800DAD80(self);
    return 0;
}
