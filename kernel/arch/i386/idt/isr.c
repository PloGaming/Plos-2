__attribute__((noreturn))
void exception_handler(void)
{
    __asm__ volatile ("cli; hlt");
}