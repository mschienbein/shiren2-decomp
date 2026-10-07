#include "common.h"
typedef struct { s32 x,y,z; } Triple;
typedef struct { Triple field_0,field_C; float field_18,field_1C; } Input;
typedef struct { Triple field_0,field_C,field_18; float field_24,field_28; } State;
extern State D_801653A0,D_801653CC;
extern Triple D_80165300,D_80165324,D_8016534C,D_80165358;
extern float D_8016533C,D_80165340;
extern s32 D_80165394,D_80165400,D_801653F8,D_801653FC,D_80165404;
extern void func_80059590(s32),func_800265E0(void *,s32),func_8005B44C(State *),func_8005C4AC(Triple *,Triple *);
extern s32 func_8005B2DC(State *,State *);
void func_8005ABBC(Input *arg,s32 first,s32 second) {
    func_80059590(0);
    D_80165394=2;
    D_80165400=0;
    func_800265E0(&D_801653A0,0x2C);
    func_800265E0(&D_801653CC,0x2C);
    D_801653A0.field_0=D_80165300;
    D_801653A0.field_18=D_80165324;
    D_801653A0.field_24=D_8016533C;
    D_801653A0.field_28=D_80165340;
    D_801653CC.field_0=arg->field_0;
    D_801653CC.field_18=arg->field_C;
    D_801653CC.field_24=arg->field_18;
    D_801653CC.field_28=arg->field_1C;
    D_801653F8=first;
    D_801653FC=0;
    D_80165404=second;
    if(!first || func_8005B2DC(&D_801653A0,&D_801653CC)) {
        func_8005B44C(&D_801653CC);
        D_801653F8=0;
        func_8005C4AC(&D_8016534C,&D_80165358);
    }
}
