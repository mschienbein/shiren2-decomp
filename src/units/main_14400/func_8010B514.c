#include "common.h"
typedef unsigned char u8;
typedef struct { unsigned short type; } Message;
typedef struct { u8 pad0[0x88]; void *field88; } Object;
typedef struct FilterVTable FilterVTable;
typedef struct { FilterVTable *vtable; Object *object; } Filter;
typedef struct Queue Queue;
extern Queue D_80140160;
extern void *D_801476B8;
extern s32 func_800EF404(Object *object, void *context);
extern s32 func_80049CB4(s32 id, ...);
extern void *func_800EB9FC(void *object);
extern Filter *func_8010B864(Filter *filter, Object *object);
extern Message *func_80093B58(Queue *queue, Filter *filter);
extern s32 func_800D8FF0(void *object);
s32 func_8010B514(Object *object, void *context) {
    Filter filter;
    Message *message;
    if (object->field88 != 0) {
        func_800EF404(object, context);
        func_80049CB4(2);
        if (func_800EB9FC(D_801476B8)) return 0;
        func_8010B864(&filter, object);
        for (;;) {
            message = func_80093B58(&D_80140160, &filter);
            if (message->type != 0x31) break;
            func_800D8FF0(message);
        }
        if (message == 0 || func_800D8FF0(message) != 0) return 0;
        else return 1;
    }
    return func_800EF404(object, context);
}
