/* Address-based names preserve the unresolved original API identity.
 * The callee forwards to func_80074140, which consumes no incoming arguments
 * and produces a signed status: 0 when its global gate permits the reset,
 * otherwise -1. Both wrappers leave that return value intact.
 */
extern int func_8005EF80(void);

int func_800413E0(void)
{
    return func_8005EF80();
}
