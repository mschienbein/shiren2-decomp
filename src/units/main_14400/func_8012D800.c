#include "common.h"

extern char D_801CA940[];
extern void func_8012D8A4(void *destination, s32 value, u32 count);
extern unsigned char *func_8002AB90(void *record, unsigned char *address, u32 span_word);
void func_8012D800(void *destination, u32 size) {
    func_8012D8A4(destination, 0, size);
    func_8002AB90(D_801CA940, destination, size);
}
