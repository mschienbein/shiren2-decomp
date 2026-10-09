#include "common.h"
typedef struct { s32 x, y; } Selection;
/* Receiver view includes its selection at +0x34 and mode at +0x80. */
typedef struct {
    unsigned char pad00[0x34];
    Selection selection;
    unsigned char pad3C[0x44];
    s32 mode;
} Object;
extern s32 func_8009BB38(Object *, Selection *);
/* Widget slot +0x34 (confirm) override in D_80152800: s32 (void *self, s32 *out).
 * The caller func_80046CB0 passes its output word (0x80046D94); this override
 * confirms from its own selection and does not read it. */
s32 func_8009BA50(Object *object, s32 *out) { return func_8009BB38(object, &object->selection) ^ 1; }
