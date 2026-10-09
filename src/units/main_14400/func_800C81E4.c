#include "common.h"
typedef struct { s32 field_00; s32 field_04[5]; } Request;
/* D_80158C98+0x5C targets E4B60, which explicitly returns message status. */
typedef struct { unsigned char field_00[0x58]; short field_58; s32 (*field_5c)(void *, Request *); } Methods;
typedef struct {
    unsigned char field_00[0x24]; Methods *field_24;
    unsigned char field_28[0x2a]; unsigned char field_52;
    unsigned char field_53[0x1f]; unsigned char field_72, field_73, field_74;
} Object;
extern unsigned char D_80140160[];
extern s32 D_80148090;
extern Object *D_801476B8;
extern s32 func_80046240(void), func_800C93CC(void), func_801F2714(void);
extern void *func_800C9E10(void);
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...), func_800452C0(void *);
extern s32 func_800E1FF4(Object *), func_800E1CD4(Object *, s32);
extern s32 func_800D7334(void), func_800D8FF0(void *);
extern void *func_80093B58(void *, void *);
extern void func_800E1048(Object *), func_80092638(void *, unsigned char);
extern u32 func_800B1C6C(Object *);
extern s32 func_800A7824(Object *, unsigned char);
s32 func_800C81E4(Object *self) {
    Request request;
    void *event;
    s32 result = 0;
    s32 blocked = 0;
    if (func_80046240() || D_801476B8->field_52) blocked = 1;
    if (blocked) {
        unsigned char state = D_801476B8->field_52;
        s32 is_three = 0;
        if (state) is_three = state == 3;
        if (is_three) {
            Object *current = D_801476B8;
            Methods *methods;
            request.field_00 = 7;
            methods = current->field_24;
            methods->field_5c((unsigned char *)current + methods->field_58, &request);
        }
    } else {
        func_80049CB4(4);
        func_80049CB4(3);
        result = 0;
        if (!func_800E1FF4(self) || func_800C93CC()) result = 1;
        if (result) {
            if (func_800E1CD4(self, 10)) {
                func_80049CB4(0x129, 10);
                func_800498E4(0x18c);
                func_80049CB4(0x129, 10);
            }
            result = 0;
            self->field_72 &= 0xfb;
        } else if (D_80148090) {
            result = func_800D7334();
            if (result == 2) func_80092638(D_80140160, 1);
        } else {
            func_80049CB4(12, 0);
            event = func_80093B58(D_80140160, 0);
            func_800452C0(func_800C9E10());
            if (event) {
                func_80049CB4(0x130, 1);
                D_801476B8->field_74++;
                result = func_800D8FF0(event);
                func_800E1048(D_801476B8);
            } else result = 1;
            func_80092638(D_80140160, result == 2);
        }
        func_80049CB4(4);
        func_80049CB4(3);
        func_80049CB4(12, 0);
        func_80049CB4(0x130, 0);
        if (result == 2) {
            if ((func_800B1C6C(self) & 0x80) && (func_800A7824(self, 2) & 0xff)) D_80148090 = 0;
            if (func_801F2714()) D_80148090 = 0;
        }
    }
    return result;
}
