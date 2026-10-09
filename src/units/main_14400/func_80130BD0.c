#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed long QueueS32;
typedef void *QueueMessage;

struct ProbeThread;
/* libultra OSMesgQueue layout (same member types as include/controller_queue_view.h). */
typedef struct {
    struct ProbeThread *receive_waiters;
    struct ProbeThread *send_waiters;
    QueueS32 valid_count;
    QueueS32 first;
    QueueS32 capacity;
    QueueMessage *messages;
} QueueView;
/* libultra OSContPad (func_800277B8 fills one per controller channel). */
typedef struct {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;

extern s32 D_801D2C0C;
extern QueueView D_801D256C;
extern QueueView D_801E028C;
extern QueueView D_801E4E80;
extern OSContPad D_801D4D08[4];
extern void (*D_80148D90)(s32 value);
extern QueueS32 func_8002FEA0(QueueView *queue, QueueMessage *message, QueueS32 flags);
extern s32 func_80027730(QueueView *queue);
extern QueueS32 func_80031D50(QueueView *queue, QueueMessage message, QueueS32 flags);
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

s32 func_80130BD0(s32 *value) {
    if (D_801D2C0C != 0) {
        return 0;
    }
    func_8002FEA0(&D_801D256C, 0, 0);
    if (request_status() == 0) {
        receive_request();
        if (!(D_801D2C0C & 1)) {
            send_completion();
            func_800277B8(D_801D4D08);
            receive_completion();
        }
    }
    if (D_80148D90 != 0) {
        D_80148D90(*value);
    }
    func_80031D50(&D_801D256C, 0, 0);
    return 0;
}
