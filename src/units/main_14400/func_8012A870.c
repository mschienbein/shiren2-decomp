#include "common.h"
typedef struct { unsigned char field00; s32 field04; } Command;
extern s32 func_8012AC2C(Command *command);
void func_8012A870(s32 value) {
    Command command;
    command.field04 = value;
    command.field00 = 1;
    func_8012AC2C(&command);
}
