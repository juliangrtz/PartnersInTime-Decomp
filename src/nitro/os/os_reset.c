#include <nitro/os_reset.h>
#include <nitro/pxi.h>

extern void OS_Terminate(void);
extern u16 data_02063020;
extern u16 data_02063024;
void OSi_ResetCallback(u32 tag, u32 data, int error);

void OS_InitReset(void)
{
    if (data_02063020) return;
    data_02063020 = 1;
    PXI_Init();
    while (!PXI_IsCallbackReady(12, 1)) {}
    PXI_SetFifoRecvCallback(12, OSi_ResetCallback);
}

void OSi_ResetCallback(u32 tag, u32 data, int error)
{
    u16 command = (data & 0x7F00) >> 8;
    if (command == 16) data_02063024 = 1;
    else OS_Terminate();
}

void OSi_SendResetCommand(u32 command)
{
    while (PXI_SendWordByFifo(12, command << 8, 0)) {}
}
