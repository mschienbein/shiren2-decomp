/* Observed unsigned byte view of existing storage; allocation ownership and
 * the higher-level meaning of bit 5 remain unresolved. This TU owns no data.
 */
extern unsigned char D_80142F1B;

int func_80042A50(void)
{
    return ((D_80142F1B >> 5) & 1) ^ 1;
}
