/* Minimal startup (vector table) placeholder; replace with STM32H563 HAL startup */
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void)) (0x20000000 + 0x000A0000), /* Initial stack pointer (placeholder) */
    0 /* Reset handler placeholder */
};
