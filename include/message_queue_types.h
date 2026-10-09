#ifndef P38_MESSAGE_QUEUE_TYPES_H
#define P38_MESSAGE_QUEUE_TYPES_H

/* Source-only reconstruction view for the resident queue family.
 * No SDK header or common.h is included. These aliases and the anonymous queue
 * reproduce both unchanged accepted controller translation units' types.
 */
typedef signed long s32;
typedef unsigned long u32;
typedef void *ProbeMessage;

struct ProbeThread;
typedef struct {
    struct ProbeThread *receive_waiters;
    struct ProbeThread *send_waiters;
    s32 valid_count;
    s32 first;
    s32 capacity;
    ProbeMessage *messages;
} ProbeMessageQueue;

/* Only next+0 and state+0x10 are accessed by these four drafts. The intervening
 * bytes deliberately have no reconstructed meaning. This completion of the
 * accepted forward tag is a partial field view, never a thread allocation,
 * context-size contract, or permission to take sizeof it for storage.
 */
struct ProbeThread {
    struct ProbeThread *next;
    unsigned char unknown_04[12];
    unsigned short state;
};

/* ABI requirements of the proposed view; none has been newly compiled. */
typedef char p38_long32[(sizeof(s32) == 4 && sizeof(u32) == 4) ? 1 : -1];
typedef char p38_pointer32[(sizeof(void *) == 4) ? 1 : -1];
typedef char p38_short16[(sizeof(unsigned short) == 2) ? 1 : -1];
typedef char p38_queue_layout[(sizeof(ProbeMessageQueue) == 24 &&
                              __alignof__(ProbeMessageQueue) == 4) ? 1 : -1];

/* Existing initialized views, with no new definition, BSS or COMMON.
 * D_80037330 is the complete 8-byte queue tail sentinel {next = 0, priority = -1}
 * (.data 0x80037330..0x80037337; D_80037338/D_8003733C hold its address). The enqueue
 * routine func_8002A794 reads +4 of every queue element (0x8002A7A0, 0x8002A7B8), including
 * this sentinel, so it is never only its first pointer word. Its address is cast to the
 * partial thread view solely for next+0.
 */
typedef struct {
    struct ProbeThread *next;
    s32 priority;
} ProbeThreadTail;
extern ProbeThreadTail D_80037330;
extern struct ProbeThread *D_80037340;

/* Call-site and original-body views. Pop's pointer and disable's mask are
 * consumed; the other helpers' incidental v0 values are ignored by this family.
 * These declarations do not recover a complete historical public API.
 */
/* Interrupt masks use common.h's unsigned-int contract, not the queue longs. */
extern unsigned int func_8002AF70(void);
extern void func_8002AFE0(unsigned int mask);
extern void func_8002A68C(struct ProbeThread **waiters);
extern struct ProbeThread *func_8002A7DC(struct ProbeThread **waiters);
extern void func_80032B50(struct ProbeThread *thread);

void func_80027EA0(ProbeMessageQueue *, ProbeMessage *, s32);
s32 func_8002B130(ProbeMessageQueue *, ProbeMessage, s32);
s32 func_8002FEA0(ProbeMessageQueue *, ProbeMessage *, s32);
s32 func_80031D50(ProbeMessageQueue *, ProbeMessage, s32);

#endif
