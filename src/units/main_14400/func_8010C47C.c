#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00, kind_01; u8 pad_02[10]; signed char base_0C, bonus_0D; } Item;
typedef struct { s32 kind; u8 pad_04[0x14]; void *field_18; } Event;
extern void func_800498E4(s32, ...);
extern char *func_800AC990(void *);
extern s32 func_800AF28C(void *, void *);
extern s32 func_8010BAE8(void *, s32);
extern s32 func_8010BD00(void *, u8);
extern s32 func_8010BDC4(void *, u8);
extern s32 func_8010BEC4(void *, u8);
extern s32 func_8010BF0C(void *);
static inline s32 event_kind(Event *event) { return event->kind; }

s32 func_8010C47C(Item *item, Event *event) {
    switch (event_kind(event)) {
    case 24: {
        s32 text = 0; /* first the protection flag, then the message id */
        if ((u8)func_8010BEC4(item, 0x43) || (u8)func_8010BEC4(item, 0x62)) text = 1;
        if (text) {
            text = 0x34;
        } else if ((u8)func_8010BEC4(item, 0x1D)) {
            text = 0x35;
        } else {
            switch (item->kind_01) {
            case 0x4F: case 0x50: case 0x5B: case 0x65: case 0x71:
                text = 0x34;
                break;
            default:
                if (item->base_0C + item->bonus_0D > 0) {
                    func_8010BAE8(item, -1);
                    text = 0x32;
                    switch (item->kind_01) {
                    case 0x32: case 0x3F: case 0x41: case 0x42: case 0x49: case 0x64: case 0x68:
                        text = 0x33;
                        break;
                    default:
                        text = 0x32;
                        break;
                    }
                } else text = 0x34;
                break;
            }
        }
        func_800498E4(text, func_800AC990(item));
        return 1;
    }
    case 25:
        if (event->field_18) {
            s32 missing = func_8010BF0C(item) != 1;
            if (missing) return func_8010BD00(item, 0xF7);
        } else if (func_8010BDC4(item, 0xF7)) {
            return 1;
        }
        return 0;
    default:
        return func_800AF28C(item, event);
    }
}
