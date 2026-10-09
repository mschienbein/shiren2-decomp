#include "common.h"
extern void func_800A30AC(void *output, void *input, unsigned char index);
void *func_800A2FD0(void *output, void *input, unsigned char index) {
    func_800A30AC(output, input, index);
    return output;
}
