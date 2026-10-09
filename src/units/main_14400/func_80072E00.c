#include "common.h"

/* Message queue at D_801A71CC (the reviewed OSMesgQueue view, see func_80072CE4). */
typedef struct {
    void *receive_waiters;
    void *send_waiters;
    long valid_count;
    long first;
    long capacity;
    void **messages;
} MesgQueue80072E00;
/* Opaque complete objects handed to the fade-track updaters (layouts live in the callees). */
typedef struct FadeTrack80072E00 FadeTrack80072E00;
typedef struct FadeTarget80072E00 FadeTarget80072E00;

extern s32 D_801A720C, D_801A7210, D_801A7214, D_801A7218, D_801A721C, D_801A7220;
extern MesgQueue80072E00 D_801A71CC;
extern FadeTrack80072E00 D_801A72A8;
extern FadeTarget80072E00 D_801A7228;
/* Fade-progress status; this update loop discards it. */
extern s32 func_8005C97C(void);
extern signed long func_80031D50(MesgQueue80072E00 *queue, void *message, signed long flags);
extern void func_80072F14(void);
extern void func_800734C0(void *first, void *second);
extern void func_80073664(void *first, void *second);
extern void func_80073D5C(void);
void func_80072E00(void) {
    func_8005C97C();
    if (D_801A7210 != 0) {
        D_801A7210--;
    }
    if (D_801A720C != 0 && (D_801A7214 == 0 || --D_801A7214 == 0) && D_801A7210 == 0) {
        func_80031D50(&D_801A71CC, 0, 0);
    }
    if (D_801A721C != 0) {
        func_80072F14();
        switch (D_801A7220) {
        case 1:
            func_800734C0(&D_801A72A8, &D_801A7228);
            break;
        case 2:
            func_80073664(&D_801A72A8, &D_801A7228);
            break;
        }
        func_80073D5C();
    }
    D_801A7218++;
}
