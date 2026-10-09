#include "common.h"

/* Countdown timer: decrement unless already zero; report expiry. */
typedef struct {
    s32 count;
} Timer80073920;

s32 func_80073920(Timer80073920 *timer) {
    if (timer->count != 0) {
        timer->count--;
    }
    return timer->count == 0;
}
