#include <game/input.h>

extern GameDisplayWork data_0206032c;
extern u8 data_02060340[1024];
extern u8 data_02060740[1024];
extern GameFrameTiming data_02060b2c;
extern const u16 data_02049fb8[10];
extern u8 SDK_AUTOLOAD_DTCM_START[];
extern void GameSystem_UpdateVBlank(void);
extern void VBlankIntrWait(void);
extern void DC_FlushRange(const void *source, u32 size);
extern void MIi_CpuClearFast(u32 value, void *destination, u32 size);
extern void func_0203b578(u32 dma, const void *source, void *destination, u32 size);

void GameInput_Read(void)
{
    u16 buttons = *(vu16 *)0x04000130;
    GameInput_Update(&data_0206032c.input, ((buttons | *(vu16 *)0x027fffa8) ^ 0x2fff) & 0x2fff);
}

void GameInput_Update(GameInput *input, u16 held)
{
    int repeated = 0;
    u32 i;
    int previous;
    input->pressed = ~input->held & held;
    input->released = input->held & ~held;
    previous = input->held;
    for (i = 0; i < 10; ++i) {
        u16 button = held & data_02049fb8[i];
        if (button) {
            if (!(previous & button)) {
                repeated |= button;
                input->countdown[i] = input->first_delay;
            } else {
                if (!--input->countdown[i]) {
                    repeated |= button;
                    input->countdown[i] = input->repeat_delay;
                }
            }
        } else {
            input->countdown[i] = 0;
        }
    }
    input->repeated = repeated;
    input->held = held;
}

void GameFrame_WaitVBlank(void)
{
    int end;
    data_02060b2c.flags |= 0x100;
    end = *(vu16 *)0x04000006;
    data_02060b2c.frame_end = end;
    if (end < data_02060b2c.frame_start) end += 263;
    if (!data_02060b2c.frame_peak_timeout || data_02060b2c.frame_peak < data_02060b2c.frame_lines) {
        data_02060b2c.frame_peak_timeout = data_02060b2c.frame_peak_hold;
        data_02060b2c.frame_peak = data_02060b2c.frame_lines;
    } else {
        --data_02060b2c.frame_peak_timeout;
    }
    data_02060b2c.frame_lines = (end - data_02060b2c.frame_start) + 263 * data_02060b2c.pending_vblanks;
    VBlankIntrWait();
    data_02060b2c.frame_start = *(vu16 *)0x04000006;
    data_02060b2c.frame_vblanks = data_02060b2c.pending_vblanks;
    data_02060b2c.pending_vblanks = 0;
    data_02060b2c.flags &= ~0x100;
}

void GameDisplay_CopyOam(u32 dma)
{
    DC_FlushRange(data_02060340, 1024);
    func_0203b578(dma, data_02060340, (void *)0x07000000, 1024);
    if (data_0206032c.display_mode != 1) {
        DC_FlushRange(data_02060740, 1024);
        func_0203b578(dma, data_02060740, (void *)0x07000400, 1024);
    }
}

void GameDisplay_ClearOam(void)
{
    volatile u32 main_fill = 512;
    MIi_CpuClearFast(main_fill, data_02060340, 1024);
    if (data_0206032c.display_mode != 1) {
        volatile u32 sub_fill = 512;
        MIi_CpuClearFast(sub_fill, data_02060740, 1024);
    }
}

void GameDisplay_SetBackgroundColor(u16 main_color, u16 sub_color)
{
    *(vu16 *)0x05000000 = main_color;
    if (data_0206032c.display_mode != 1) *(vu16 *)0x05000400 = sub_color;
}

void GameFrame_VBlank(void)
{
    int end;
    data_02060b2c.irq_start = *(vu16 *)0x04000006;
    GameSystem_UpdateVBlank();
    ++data_02060b2c.pending_vblanks;
    end = *(vu16 *)0x04000006;
    data_02060b2c.irq_end = end;
    if (end < data_02060b2c.irq_start) end += 263;
    data_02060b2c.irq_lines = end - data_02060b2c.irq_start;
    if (!data_02060b2c.irq_peak_timeout || data_02060b2c.irq_peak < data_02060b2c.irq_lines) {
        data_02060b2c.irq_peak_timeout = data_02060b2c.irq_peak_hold;
        data_02060b2c.irq_peak = data_02060b2c.irq_lines;
    } else {
        --data_02060b2c.irq_peak_timeout;
    }
    *(vu32 *)((u32)SDK_AUTOLOAD_DTCM_START + 0x3ff8) |= 1;
}
