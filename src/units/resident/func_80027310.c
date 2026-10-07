/* Independently reconstructed from the frozen original instructions.
 * func_80027AAC writes a byte at arg0 and records at arg1. The original
 * SDK names, full buffer type, and return-value contract remain unresolved.
 * This wrapper ignores the callee's return value and its local output byte.
 */
#include "controller_status.h"

extern void func_80027AAC(unsigned char *arg0, ProbeControllerStatus *arg1);

void func_80027310(ProbeControllerStatus *arg0)
{
    unsigned char sp10;

    func_80027AAC(&sp10, arg0);
}
