#include "common.h"
typedef struct Obj Obj;
extern void *D_80148D80;
extern void *D_80148D84;
extern void *D_801DFF54;
extern void func_8012CE28(Obj *a);
extern void func_8012C74C(Obj *a);
void func_8012C6E0(void *manager, Obj *config) {
    if (!D_80148D80) {
        D_80148D80 = manager;
        if (!D_80148D84) {
            func_8012CE28(config);
            D_801DFF54 = D_80148D80;
            D_80148D84 = D_80148D80;
            func_8012C74C(config);
        }
    }
}
