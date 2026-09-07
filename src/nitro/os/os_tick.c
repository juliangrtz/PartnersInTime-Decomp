#include <nitro/os_sync.h>

extern u16 data_02062fec;
extern u16 data_02062ff0;
extern int data_02062ff4;
extern volatile u64 data_02062ff8;
extern void func_02038cf8(int timer, void (*callback)(void *), void *argument);
extern void OS_SetIrqFunction(u32 mask, void (*callback)(void *));
extern u32 OS_EnableIrqMask(u32 mask);
void func_0203a6fc(int timer);
void func_0203a7c8(void *argument);

#define TIMER0_COUNT (*(vu16 *)0x04000100)
#define TIMER0_CONTROL (*(vu16 *)0x04000102)
#define IRQ_REQUEST (*(vu32 *)0x04000214)

void func_0203a850(void) {
    if (data_02062ff0) return;
    data_02062ff0 = 1;
    func_0203a6fc(0);
    data_02062ff8 = 0;
    TIMER0_CONTROL = 0;
    TIMER0_COUNT = 0;
    TIMER0_CONTROL = 0xC1;
    OS_SetIrqFunction(8, func_0203a7c8);
    OS_EnableIrqMask(8);
    data_02062ff4 = 0;
}

void func_0203a7c8(void *argument) {
    data_02062ff8++;
    if (data_02062ff4) {
        TIMER0_CONTROL = 0;
        TIMER0_COUNT = 0;
        TIMER0_CONTROL = 0xC1;
        data_02062ff4 = 0;
    }
    func_02038cf8(0, func_0203a7c8, 0);
}

/* The 16-bit timer and its 48-bit overflow count form one 64-bit tick. An
 * interrupt pending before the overflow callback can require one carry. */
u64 func_0203a718(void) {
    u32 state = OS_DisableInterrupts();
    volatile u16 counter = TIMER0_COUNT;
    volatile u64 overflow = data_02062ff8 & 0x0000FFFFFFFFFFFFULL;
    if ((IRQ_REQUEST & 8) && !(counter & 0x8000)) overflow++;
    OS_RestoreInterrupts(state);
    return (overflow << 16) | counter;
}

void func_0203a6fc(int timer) {
    data_02062fec |= 1 << timer;
}
