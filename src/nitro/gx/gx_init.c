#include <nitro/gx_init.h>

extern u16 data_02059d74;
extern int data_02059d78;
extern u16 data_02062c48;
extern u16 data_02062c4c;
extern void func_02035444(void);
extern int OS_GetLockID(void);
extern void OS_Terminate(void);
extern void func_0203b5f4(u32 channel, void *destination, u32 value, u32 size);
extern void func_0203b7a0(u32 value, void *destination, u32 size);

#define DISP_CONTROL (*(vu32 *)0x04000000)
#define DISP_STATUS (*(vu16 *)0x04000004)
#define POWER_CONTROL (*(vu16 *)0x04000304)

void GX_Init(void)
{
    POWER_CONTROL |= 0x8000;
    POWER_CONTROL = (POWER_CONTROL & ~0x20e) | 0x20e;
    POWER_CONTROL |= 1;
    func_02035444();
    if (!data_02062c48) {
        do {
            int lock = OS_GetLockID();
            if (lock == -3) OS_Terminate();
            data_02062c48 = lock;
        } while (!data_02062c48);
    }
    DISP_STATUS = 0;
    DISP_CONTROL = 0;
    if (data_02059d78 != -1) {
        func_0203b5f4(data_02059d78, (void *)0x04000008, 0, 96);
        *(vu16 *)0x0400006c = 0;
        func_0203b5f4(data_02059d78, (void *)0x04001000, 0, 112);
    } else {
        volatile u32 main_zero = 0;
        func_0203b7a0(main_zero, (void *)0x04000008, 96);
        *(vu16 *)0x0400006c = 0;
        {
            volatile u32 sub_zero = 0;
            func_0203b7a0(sub_zero, (void *)0x04001000, 112);
        }
    }
    *(vu16 *)0x04000020 = 256;
    *(vu16 *)0x04000026 = 256;
    *(vu16 *)0x04000030 = 256;
    *(vu16 *)0x04000036 = 256;
    *(vu16 *)0x04001020 = 256;
    *(vu16 *)0x04001026 = 256;
    *(vu16 *)0x04001030 = 256;
    *(vu16 *)0x04001036 = 256;
}

void GX_SetVCountEqVal(int line)
{
    DISP_STATUS = (DISP_STATUS & 0x3f) | ((line & 255) << 8) | ((line & 0x100) >> 1);
}

int GX_HBlankIntr(int enable)
{
    int previous = DISP_STATUS & 16;
    if (enable) DISP_STATUS |= 16;
    else DISP_STATUS &= ~16;
    return previous;
}

int GX_VBlankIntr(int enable)
{
    int previous = DISP_STATUS & 8;
    if (enable) DISP_STATUS |= 8;
    else DISP_STATUS &= ~8;
    return previous;
}

void GX_DispOff(void)
{
    u32 control = DISP_CONTROL;
    data_02059d74 = 0;
    data_02062c4c = (control & 0x30000) >> 16;
    DISP_CONTROL = control & ~0x30000;
}

void GX_DispOn(void)
{
    data_02059d74 = 1;
    if (data_02062c4c) DISP_CONTROL = (DISP_CONTROL & ~0x30000) | (data_02062c4c << 16);
    else DISP_CONTROL |= 0x10000;
}

void GX_SetGraphicsMode(int display, int background, int background0_3d)
{
    u32 control = DISP_CONTROL;
    data_02062c4c = display;
    if (!data_02059d74) display = 0;
    DISP_CONTROL = background | ((control & 0xfff0fff0) | (display << 16)) | (background0_3d << 3);
    if (!data_02062c4c) data_02059d74 = 0;
}

void GXS_SetGraphicsMode(int background)
{
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~7) | background;
}

void GXi_SetMasterBrightness(vu16 *register_address, int brightness)
{
    if (!brightness) *register_address = 0;
    else if (brightness > 0) *register_address = brightness | 0x4000;
    else *register_address = -brightness | 0x8000;
}

void func_020350d0(void)
{
}
