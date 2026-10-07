/* A word view of the observed SW/LW slot, not an original owner/type name.
 * Known callers use zero and one; these wrappers preserve all 32 bits.
 * The original slot lifetime and higher-level meaning remain unresolved.
 */
typedef unsigned int SlotWord;

extern SlotWord D_80139614;

void func_80048ABC(SlotWord value)
{
    D_80139614 = value;
}

SlotWord func_80048AC8(void)
{
    return D_80139614;
}

typedef char check_slot_word_width[(sizeof(SlotWord) == 4) ? 1 : -1];
