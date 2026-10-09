#include "common.h"

typedef unsigned char u8;

typedef struct Obj Obj;

extern u8 D_80143392;

void func_800B8628(Obj *obj, u8 flag);
void func_800B8B34(Obj *obj);
void func_800B91AC(Obj *obj);
void func_800B95DC(Obj *obj);
void func_800B17A4(void);

void func_800C22A0(Obj *obj) {
    func_800B8628(obj, 1);
    func_800B8B34(obj);
    func_800B91AC(obj);
    func_800B95DC(obj);
    func_800B17A4();
    D_80143392 = 0;
}
