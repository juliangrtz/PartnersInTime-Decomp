/* Initialize graphics, interrupts and frame state before entering the game loop. */
#include <game/input.h>
#include <game/task.h>
#include <nitro/gx_init.h>
#include <nitro/rtc.h>

typedef struct BootDisplayWork {
    GameDisplayWork display;
    int console_type_24_25;
    int console_type_00_01;
} BootDisplayWork;
typedef char BootDisplayWorkSize[sizeof(BootDisplayWork) == 0x840 ? 1 : -1];
extern BootDisplayWork data_0206032c;
extern u8 data_02060340[1024], data_02060740[1024];
extern GameFrameTiming data_02060b2c;
extern void OS_SetIrqFunction(u32, void (*)(void));
extern u32 OS_EnableIrqMask(u32);
extern void func_0203b5f4(u32, void *, u32, u32);
extern void MI_CpuFill8(void *, u8, u32);
extern u32 func_02039b34(void);
extern void func_0200547c(void);

void main(void)
{
    u32 console_type;
    GX_Init();
    *(vu16 *)0x04000304 = (*(vu16 *)0x04000304 & ~0x20e) | 0x20e;
    GX_DispOff();
    *(vu32 *)0x04001000 &= ~0x10000;
    func_020350d0();
    OS_SetIrqFunction(1, GameFrame_VBlank);
    OS_EnableIrqMask(1);
    /* The native IRQ-enable sequence reads the old master-enable halfword. */
    (void)*(vu16 *)0x04000208;
    *(vu16 *)0x04000208 = 1;
    GX_VBlankIntr(1);
    RTC_Init();
    GameTask_InitSystem();
    GameIrqTask_InitSystem();
    func_0203b5f4(3, data_02060340, 512, 1024);
    func_0203b5f4(3, data_02060740, 512, 1024);
    MI_CpuFill8(&data_0206032c.display.input, 0, sizeof(GameInput));
    data_0206032c.display.input.first_delay = 10;
    data_0206032c.display.input.repeat_delay = 2;
    data_02060b2c.frame_peak_hold = 60;
    data_02060b2c.irq_peak_hold = 60;
    /* Preserve all default cases when decoding the SDK console-type flags. */
    console_type = func_02039b34();
    switch (console_type & 0xf0000000) {
    case 0x10000000:
        data_0206032c.display.display_mode = 0;
        break;
    case 0x20000000:
        data_0206032c.display.display_mode = 1;
        break;
    case 0x40000000:
        data_0206032c.display.display_mode = 2;
        break;
    case 0x80000000:
        data_0206032c.display.display_mode = 3;
        break;
    default:
        data_0206032c.display.display_mode = 4;
        break;
    }
    switch (console_type & 0x03000000) {
    case 0x01000000:
        data_0206032c.console_type_24_25 = 0;
        break;
    case 0x02000000:
        data_0206032c.console_type_24_25 = 1;
        break;
    default:
        data_0206032c.console_type_24_25 = 2;
        break;
    }
    switch (console_type & 3) {
    case 1:
        data_0206032c.console_type_00_01 = 0;
        break;
    case 2:
        data_0206032c.console_type_00_01 = 1;
        break;
    default:
        data_0206032c.console_type_00_01 = 2;
        break;
    }
    func_0200547c();
}
