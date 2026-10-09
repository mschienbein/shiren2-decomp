#include "common.h"

extern s32 D_8013CA30, D_8013CA34, D_8013CA38, D_8013CA3C;

/* Debug option parser: splits the command line in place at spaces (the
 * debugger supplies at most 31 arguments) and applies -d/-v/-s/-l flags. */
void func_8006AD60(char *command) {
    char *arguments[32];
    s32 count = 1;
    char **argument = arguments;

    if (!command || !*command) return;
    for (;;) {
        while (*command == ' ') *command++ = 0;
        if (!*command) break;
        arguments[count++] = command;
        while (*command && *command != ' ') ++command;
        if (!*command) break;
    }
    if (count < 2) return;
    do {
        if (argument[1][0] != '-') return;
        switch (argument[1][1]) {
        case 'd': D_8013CA34 = 1; break;
        case 'v': D_8013CA30 = 1; break;
        case 's': D_8013CA38 = 1; break;
        case 'l': D_8013CA3C = 1; break;
        }
        --count;
        ++argument;
    } while (count >= 2);
}
