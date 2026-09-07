#include <nitro.h>

typedef void (*IrqCallbackFunction)(void *argument);
typedef struct IrqCallback {
    IrqCallbackFunction function;
    u32 enabled;
    void *argument;
} IrqCallback;

extern IrqCallback OSi_IrqCallbackInfo[];
extern IrqCallback data_02062cd0[];
typedef struct OsDtcm {
    u16 irq_depth;
    u8 unknown02[0x3FF6];
    volatile u32 irq_check;
} OsDtcm;
extern OsDtcm SDK_AUTOLOAD_DTCM_START;
extern IrqCallbackFunction OS_IRQTable[];
extern const u16 data_02059d7c[];

#define IRQ_MASTER (*(vu16 *)0x04000208)
#define IRQ_ENABLE (*(vu32 *)0x04000210)
#define IRQ_REQUEST (*(vu32 *)0x04000214)
#define IRQ_CHECK (*(vu32 *)((u32)&SDK_AUTOLOAD_DTCM_START + 0x3FF8))

u32 OS_EnableIrqMask(u32 mask);
u32 OS_DisableIrqMask(u32 mask);

void func_02038e20(void) {
    SDK_AUTOLOAD_DTCM_START.irq_depth = 0;
}

void OS_SetIrqFunction(u32 mask, IrqCallbackFunction function) {
    int index;
    for (index = 0; index < 22; index++, mask >>= 1) {
        if (mask & 1) {
            IrqCallback *callback = 0;
            if (index >= 8 && index <= 11) {
                callback = &OSi_IrqCallbackInfo[index - 8];
            } else if (index >= 3 && index <= 6) {
                callback = &OSi_IrqCallbackInfo[index + 1];
            } else {
                OS_IRQTable[index] = function;
            }
            if (callback) {
                callback->function = function;
                callback->argument = 0;
                callback->enabled = 1;
            }
        }
    }
}

void func_02038d44(int channel, IrqCallbackFunction function, void *argument) {
    OSi_IrqCallbackInfo[channel].function = function;
    OSi_IrqCallbackInfo[channel].argument = argument;
    OSi_IrqCallbackInfo[channel].enabled = OS_EnableIrqMask(1 << (channel + 8)) &
                              (1 << (channel + 8));
}

void func_02038cf8(int timer, IrqCallbackFunction function, void *argument) {
    data_02062cd0[timer].function = function;
    data_02062cd0[timer].argument = argument;
    OS_EnableIrqMask(1 << (timer + 3));
    data_02062cd0[timer].enabled = 1;
}

u32 func_02038cc4(u32 mask) {
    u16 master = IRQ_MASTER;
    u32 previous;
    IRQ_MASTER = 0;
    previous = IRQ_ENABLE;
    IRQ_ENABLE = mask;
    (void)IRQ_MASTER;
    IRQ_MASTER = master;
    return previous;
}

u32 OS_EnableIrqMask(u32 mask) {
    u16 master = IRQ_MASTER;
    u32 previous;
    IRQ_MASTER = 0;
    previous = IRQ_ENABLE;
    IRQ_ENABLE = previous | mask;
    (void)IRQ_MASTER;
    IRQ_MASTER = master;
    return previous;
}

u32 OS_DisableIrqMask(u32 mask) {
    u16 master = IRQ_MASTER;
    u32 previous;
    IRQ_MASTER = 0;
    previous = IRQ_ENABLE;
    IRQ_ENABLE = previous & ~mask;
    (void)IRQ_MASTER;
    IRQ_MASTER = master;
    return previous;
}

u32 OS_ResetRequestIrqMask(u32 mask) {
    u16 master = IRQ_MASTER;
    u32 previous;
    IRQ_MASTER = 0;
    previous = IRQ_REQUEST;
    IRQ_REQUEST = mask;
    (void)IRQ_MASTER;
    IRQ_MASTER = master;
    return previous;
}

void func_02038c0c(void) {}

void func_02038b70(int index) {
    u32 mask = 1 << data_02059d7c[index];
    IrqCallbackFunction function = OSi_IrqCallbackInfo[index].function;
    OSi_IrqCallbackInfo[index].function = 0;
    if (function) function(OSi_IrqCallbackInfo[index].argument);
    IRQ_CHECK |= mask;
    if (!OSi_IrqCallbackInfo[index].enabled) OS_DisableIrqMask(mask);
}

void func_02038b60(void) { func_02038b70(0); }
void func_02038b50(void) { func_02038b70(1); }
void func_02038b40(void) { func_02038b70(2); }
void func_02038b30(void) { func_02038b70(3); }
void func_02038b20(void) { func_02038b70(4); }
void func_02038b10(void) { func_02038b70(5); }
void func_02038b00(void) { func_02038b70(6); }
void func_02038af0(void) { func_02038b70(7); }
