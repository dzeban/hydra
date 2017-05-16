/** Issue xchg %bx %bx - bochs magic breakpoint */
void __BREAK_BOCHS(void)
{
    asm volatile ("xchg %bx, %bx");
}
