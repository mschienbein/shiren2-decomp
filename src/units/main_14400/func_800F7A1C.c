#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    char reserved_00[0x4C];
    const void *methods_4C;
    char reserved_50[0x10];
} Dialog;
typedef struct {
    Dialog base;
    char reserved_60[8];
    s32 field_68;
    void *field_6C;
    char reserved_70[0xA0];
} ExtendedDialog;
typedef struct {
    char reserved_00[0xA8];
    s32 field_A8;
    unsigned char field_AC;
} State;
/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];
extern void *D_801476B8;
extern s32 D_80148390[], D_80138FE0[], D_80138FF0[];
extern s32 D_8014AB2C[], D_8014AB3C[];
extern s32 D_80151EC8[];
extern const unsigned char D_80151E38[144], D_801521D0[144], D_80152AE8[144];
extern void func_801F212C(u16 id, u8 flag);
extern s32 func_801EF340(s32, s32);
extern char *func_800A3B20(State *);
extern void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32);
extern s32 func_80049CB4(s32 command, ...);
/* Both helpers return the allocated action bundle, or null. */
extern void *func_800F7C98(State *, s32);
extern void *func_800F8060(State *);
extern void *func_800953C0(void *);
extern void func_80097240(void *, void *, void *, void *);
extern s32 func_800957C0(void *, s32 *, s32, void *, s32);
extern s32 func_800EB820(void *);
extern char *func_80048480(u16);
extern s32 func_8005EF08(char *, const char *, ...);
extern void func_8009D610(void *, char *, void *, void *);

static inline void initialize_dialog(Dialog *dialog) {
    func_800953C0(dialog);
    dialog->methods_4C = D_801521D0;
}

static inline void reset_dialog(Dialog *dialog) {
    dialog->methods_4C = D_80151E38;
}

static inline void initialize_confirmation(Dialog *dialog) {
    func_800953C0(dialog);
    dialog->methods_4C = D_80152AE8;
}

static inline s32 dialog_cancelled(void *dialog, s32 *selection) {
    return func_800957C0(dialog, selection, 1, 0, 0) ^ 1;
}

/* Returns the action bundle produced by func_800F8060/func_800F7C98, or null when cancelled. */
void *func_800F7A1C(State *state) {
    Dialog dialog;
    s32 selection[2];
    char message[200];
    ExtendedDialog confirmation;
    s32 message_id;
    void *result;
    char *name;
    s32 amount;
    s32 *selected;
    if (state->field_AC != 0) {
        func_801F212C(0x290, 1);
        func_801EF340(D_80140160[4] == 1, 0);
    }
    switch (state->field_AC) {
    case 1:
        message_id = 0x1D9;
        break;
    case 2:
        message_id = 0x1D8;
        break;
    default:
        name = func_800A3B20(state);
        func_800498E4(state->field_A8 ? 0x1D7 : 0x1D6, name);
        func_80049BF0(0);
        func_80049CB4(2);
        return func_800F7C98(state, 1);
    }
    func_800498E4(message_id, func_800A3B20(state));
    func_80049BF0(0);
    func_80049CB4(2);
    initialize_dialog(&dialog);
    func_80097240(&dialog, D_80148390, D_80138FE0, D_80138FF0);
    selected = selection;
    if (dialog_cancelled(&dialog, selected)) {
        selection[0] = 2;
    }
    switch (selection[0]) {
    case 1:
        result = func_800F8060(state);
        reset_dialog(&dialog);
        return result;
    case 0:
        selection[0] = 0;
        amount = func_800EB820(D_801476B8);
        if (amount > 0) {
            func_8005EF08(message, func_80048480(0x299), amount);
            initialize_confirmation(&confirmation.base);
            confirmation.field_68 = -1;
            confirmation.field_6C = D_80151EC8;
            func_8009D610(&confirmation, message, D_8014AB2C, D_8014AB3C);
            if (dialog_cancelled(&confirmation, selected)) {
                selection[0] = 0;
            }
            reset_dialog(&confirmation.base);
        }
        result = func_800F7C98(state, selection[0] != 0);
        reset_dialog(&dialog);
        return result;
    default:
        func_800498E4(0x1DF, func_800A3B20(state));
        reset_dialog(&dialog);
        return 0;
    }
}
