#include "common.h"
#include "controller_queue_view.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
/* libultra OSContPad (func_800277B8 fills one per controller channel). */
typedef struct {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;
extern ControllerQueueView D_801D256C;
extern ControllerQueueView D_801E028C;
extern ControllerQueueView D_801E4E80;
extern OSContPad D_801D4D08[4];
extern void (*D_80148D90)(s32 value);
extern ControllerQueueS32 func_8002FEA0(ControllerQueueView *queue, ControllerQueueMessage *message, ControllerQueueS32 flags);
extern s32 func_80027730(ControllerQueueView *queue);
extern ControllerQueueS32 func_80031D50(ControllerQueueView *queue, ControllerQueueMessage message, ControllerQueueS32 flags);
extern void func_800277B8(OSContPad *data);
static inline s32 request_status(void) {
    return func_80027730(&D_801E028C);
}
/* The queue statuses below are intentionally ignored; no message is read. */
static inline void receive_request(void) {
    func_8002FEA0(&D_801E028C, 0, 1);
}
static inline void send_completion(void) {
    func_80031D50(&D_801E4E80, 0, 1);
}
static inline void receive_completion(void) {
    func_8002FEA0(&D_801E4E80, 0, 1);
}
static inline s32 wait_for_request(void) {
    s32 status = request_status();
    if (status != 0) {
        return status;
    }
    receive_request();
    send_completion();
    func_800277B8(D_801D4D08);
    receive_completion();
    return 0;
}
s32 func_80130D30(s32 *value) {
    s32 status;
    func_8002FEA0(&D_801D256C, 0, 0);
    status = wait_for_request();
    if (status == 0) {
        if (D_80148D90 != 0) {
            D_80148D90(*value);
        }
        return 0;
    }
    return status;
}
