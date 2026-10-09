#include "common.h"

/* One of the two consecutive 0x18-byte region records (owner byte +0, area pointer +4; see
 * func_800D3C68), constructed by func_800B1080. Its destructor is declared but empty. */
struct Region800B6518 {
    signed char owner;
    unsigned char pad01[3];
    void *area;
    unsigned char pad08[0x10];
    ~Region800B6518() {}
};

extern "C" Region800B6518 D_80143330[2];

/* Static destructor for D_80143330[2] in the g++ 2.8.1 vector-delete shape: null check, then
 * destroy the elements from the end. Each inline destructor call reserves the 16-byte
 * outgoing-argument frame. */
extern "C" void func_800B6518(void)
{
    if (D_80143330 != 0) {
        Region800B6518 *end = D_80143330 + 2;

        /* ODD_C: the vector-delete loop tests before destroying (while semantics); spelled as
         * a guard plus do/while because a g++ while condition opens its own binding level,
         * which stops GCC rotating the loop the way the compiler-generated loop is rotated. */
        if (end != D_80143330) {
            do {
                --end;
                end->~Region800B6518();
            } while (end != D_80143330);
        }
    }
}
