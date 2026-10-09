#include "common.h"
typedef struct Object Object;
extern void func_8012EEA4(Object *object, s32 action, void *data);
s32 func_8012F32C(Object *object, s32 action, void *data) {
    func_8012EEA4(object, action, data);
    return 0;
}
