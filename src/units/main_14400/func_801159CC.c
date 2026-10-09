#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Point;
typedef struct { u8 value; } Dir;
typedef union { u32 bits; struct { u32 high : 8; u32 blocked : 1; u32 low : 23; } fields; } Flags;
typedef struct { u8 field_0, code, flags_2; u8 pad3[2]; signed char field_5; u8 pad6[6]; u8 flags_C; } Obj;
typedef struct { Point position; Dir direction; u8 pad9[0x15]; u8 flags_1E; u8 pad1F; Flags flags_20; } Unit;
typedef Unit Obj_80115E40;
typedef Unit UnkFunc800A6E50Arg0;
typedef Obj UnkFunc800A6E50Arg1;
typedef Unit Object;
typedef struct { u8 pad0[8]; Unit *unit_8; Dir direction_C; u8 padD[3]; Point position_10; s32 field_18; } Msg;
extern Unit *D_801476B8;
extern s32 func_800C94D8(void);
extern s32 func_80116928(unsigned char);
extern s32 func_800ADC90(void *, void *, void *);
extern u32 func_8011575C(void *);
extern s32 func_80049CB4(s32, ...);
extern char *func_800AC990(void *);
extern void func_800497F0(s32, ...);
extern void func_80115DC8(Obj *, Unit *, Point *, u8);
extern Obj_80115E40 *func_80115E40(void *, void *);
extern s32 func_800A44F4(void *, void *);
extern int func_800A6E50(UnkFunc800A6E50Arg0 *, UnkFunc800A6E50Arg1 *);
/* a1 reaches virtual slot 0x50; target 800E22E8 switches on that mode. */
extern s32 func_800A6F98(Object *, s32 mode);
extern char *func_800A3B20(Unit *);
extern void func_80049AE8(s32, ...);
extern s32 func_80115EB0(Obj *, Unit *, Point *, Dir *, Unit *, Unit *);
extern void func_801169D0(void);
static inline s32 snapshot_flag(Flags *out, Unit *obj) {
    *out = obj->flags_20;
    return out->fields.blocked;
}
void func_801159CC(Obj *obj, s32 action, Msg *message) {
    Point position;
    Flags targetFlags;
    Flags playerFlags;
    Unit *player;
    s32 ready = 0;
    if (!((obj->flags_C >> 2) & 1) && !~obj->field_5) ready = func_800C94D8() != 0;
    if (!ready || !func_80116928(obj->code)) {
        if (action == 0x17) func_800ADC90(message->unit_8, &message->position_10, &message->position_10);
        return;
    }
    player = func_8011575C(obj) ? D_801476B8 : 0;
    if (action == 0x15) {
        Point *p = &position;
        s32 event;
        char *name;
        Dir *direction;
        Unit *target;
        position = message->position_10;
        event = func_80049CB4(0xDA, p);
        name = func_800AC990(obj);
        func_800497F0(0xEE, event, name);
        func_80115DC8(obj, player, p, 1);
        direction = &message->direction_C;
        target = func_80115E40(obj, p);
        func_80115EB0(obj, player, p, direction, target, 0);
    } else if (action == 0x16) {
        s32 allowed = 0;
        Unit *target = message->unit_8;
        s32 event;
        s32 targetFlag, playerFlag;
        position = target->position;
        event = func_80049CB4(0xDA, &position);
        targetFlag = snapshot_flag(&targetFlags, target);
        playerFlag = snapshot_flag(&playerFlags, D_801476B8);
        if ((target->flags_1E >> 2) & 1) {
            s32 condition = 0;
            if (message->field_18 || !targetFlag || ((obj->flags_C >> 1) & 1)) condition = 1;
            if (condition) allowed = 1;
        } else if (playerFlag && func_800A44F4(player, target) != 1) allowed = 1;
        if (allowed) {
            s32 state = func_800A6E50(target, obj);
            s32 check;
            func_80115DC8(obj, player, &position, 0);
            check = 0;
            if (!message->field_18) {
                s32 blocked = (obj->flags_C >> 1) & 1;
                check = blocked == 0;
            }
            if (check) allowed = func_800A6F98(target, state ? 3 : 2) ^ 1;
            if (allowed) {
                if (target->flags_1E & 0xC) {
                    char *name = func_800A3B20(target);
                    char *objectName = func_800AC990(obj);
                    func_80049AE8(0xEC, event, name, objectName);
                } else {
                    char *name = func_800A3B20(target);
                    char *objectName = func_800AC990(obj);
                    func_800497F0(0xED, event, name, objectName);
                }
                func_80115EB0(obj, player, &position, &target->direction, target, 0);
            } else if (target->flags_1E & 0xC) {
                char *name = func_800A3B20(target);
                char *objectName = func_800AC990(obj);
                func_80049AE8(0xEC, event, name, objectName);
                func_800497F0(0xEF, event);
            }
        }
    } else if (action == 0x17) {
        Point *p = &position;
        s32 event;
        char *name;
        Dir *direction;
        Unit *target, *source;
        position = message->position_10;
        event = func_80049CB4(0xDA, p);
        func_80049CB4(6);
        name = func_800AC990(obj);
        func_800497F0(0xEE, event, name);
        func_80049CB4(7);
        func_80115DC8(obj, player, p, 0);
        direction = &message->direction_C;
        target = func_80115E40(obj, p);
        source = message->unit_8;
        func_80115EB0(obj, player, p, direction, target, source);
    }
    func_801169D0();
}
