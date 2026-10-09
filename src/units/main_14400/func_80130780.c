#include "common.h"
typedef struct Message { struct Message *next; } Message;
typedef struct { unsigned char pad0[0x2C]; Message *head; } Queue;
extern Queue *D_80148D84;
Message *func_80130780(void) {
    Message *message = 0;
    if (D_80148D84->head) {
        message = D_80148D84->head;
        D_80148D84->head = message->next;
        message->next = 0;
    }
    return message;
}
