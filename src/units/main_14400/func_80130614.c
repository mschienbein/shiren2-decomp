#include "common.h"

typedef struct {
    s32 left;
    s32 right;
} Frame;

/* Same player client layout as the registration in func_80129C2C. */
typedef struct Callback {
    struct Callback *next;
    void *clientData;
    s32 (*update)(struct Callback *);
    s32 field_0C;
    s32 end;
} Voice;

typedef struct {
    Voice *voice;
    unsigned char pad4[0x18];
    s32 unk1C;
    s32 pos;
    unsigned char pad24[0x20];
    s32 maxChunk;
    s32 *unk48;
} Mixer;

extern Mixer *D_80148D84;
extern s32 func_8013085C(s32 arg0);
extern Frame *func_801308E0(s32 pos, Frame *out);
extern void func_801307C4(void);

Frame *func_80130614(Frame *buf, s32 *outCount, s32 *samples, s32 count) {
    Voice *voice;
    Frame *out = buf;
    s32 *src = samples;
    s32 chunk;

    if (D_80148D84->voice == 0) {
        *outCount = 0;
        return out;
    }
    voice = D_80148D84->voice;
    while (voice->end - D_80148D84->pos < count) {
        D_80148D84->unk1C = voice->end & ~0xF;
        voice->end += func_8013085C(voice->update(voice));
    }
    D_80148D84->unk1C &= ~0xF;
    while (count > 0) {
        chunk = count;
        if (D_80148D84->maxChunk < count) {
            chunk = D_80148D84->maxChunk;
        }
        D_80148D84->unk48 = src;
        out = func_801308E0(D_80148D84->pos, out);
        count -= chunk;
        src += chunk;
        D_80148D84->pos += chunk;
    }
    *outCount = out - buf;
    func_801307C4();
    return out;
}
