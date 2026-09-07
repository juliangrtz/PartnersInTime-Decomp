#include <nitro/pm.h>

extern u16 data_02064cc4;
extern volatile u32 data_02064cc8;
extern volatile u32 data_02064ccc;
extern u32 data_02064cd4;
extern OsMutex data_02064cec;
extern void PXI_Init(void);
extern int PXI_IsCallbackReady(u32 tag, int processor);
extern void PXI_SetFifoRecvCallback(u32 tag, void (*callback)(u32, u32, int));
extern int PXI_SendWordByFifo(u32 tag, u32 data, int error);
extern void func_02039f58(OsMutex *mutex);
extern u32 OS_DisableIrqMask(u32 mask);
extern u32 func_02038cc4(u32 mask);
extern int CTRDG_IsExisting(u32 value);
extern void GX_DispOff(void);
extern void GX_DispOn(void);
extern void OS_Halt(void);
extern void func_0203ae6c(u32 cycles);

#define PM_POWER (*(vu16 *)0x04000304)
#define PM_IME (*(vu16 *)0x04000208)
#define PM_FRAME (*(vu32 *)0x027FFC3C)

static inline u32 PMi_GetFrame(void) { return PM_FRAME; }
static inline u32 PMi_ReplyValue(u16 result) { return (u8)result; }

int PMi_Lock(void) {
    u32 state = OS_DisableInterrupts();
    if (data_02064cdc.busy) {
        OS_RestoreInterrupts(state);
        return 0;
    }
    data_02064cdc.busy = 1;
    OS_RestoreInterrupts(state);
    return 1;
}
u32 PMi_WaitBusy(void) {
    u32 busy;
    do { busy = data_02064cdc.busy; } while (busy);
    return busy;
}
void PMi_SetSyncResult(u32 result, void *output) { *(u32 *)output = result; }

void PMi_Complete(u32 result) {
    PmCallback callback = data_02064cdc.callback;
    void *argument = data_02064cdc.argument;
    if (data_02064cdc.busy) data_02064cdc.busy = 0;
    if (callback) {
        data_02064cdc.callback = 0;
        callback(result, argument);
    }
}

void PM_Init(void) {
    int i;
    if (data_02064cc4) return;
    data_02064cc4 = 1;
    data_02064cdc.busy = 0;
    data_02064cdc.callback = 0;
    PXI_Init();
    while (!PXI_IsCallbackReady(8, 1)) {}
    PXI_SetFifoRecvCallback(8, PMi_FifoCallback);
    for (i = 0; i < 4; i++) data_02064d00[i].ready = 0;
    func_02039f58(&data_02064cec);
    data_02064cd4 = PM_FRAME;
}

void PMi_FifoCallback(u32 tag, u32 data, int error) {
    u16 command;
    u16 result;
    if (error) { PMi_Complete(2); return; }
    command = (data & 0x7F00) >> 8;
    result = data & 0xFF;
    if (command >= 0x70 && command <= 0x73) {
        u32 index = command - 0x70;
        u16 value = PMi_ReplyValue(result);
        u16 *output = *(u16 **)((u8 *)data_02064d04 + index * 8);
        if (output) *output = value;
        data_02064d00[index].ready = 1;
        result = 0;
    } else {
        if (command == 0x60) data_02064cc8 = 1;
        else if (command == 0x62) data_02064ccc = 1;
        else if (command == 0x67) {
            if (data_02064cdc.shutdown_result) *data_02064cdc.shutdown_result = result;
            result = 0;
        }
    }
    PMi_Complete(result);
}

int PMi_SendSleepStart(u16 triggers, u16 keys) {
    if (!PMi_Lock()) return 1;
    data_02064cc8 = 0;
    PMi_SendPxiData(0x03006000);
    while (!data_02064cc8) {}
    data_02064cc8 = 0;
    data_02064ccc = 0;
    PM_SetLCDPower(0, 2, 0, 0);
    PMi_SendPxiData((u8)triggers | 0x02006100);
    PMi_SendPxiData((keys & 0xFFFF) | 0x01010000);
    return 0;
}

int PMi_SendUtilityCommand(u32 command, PmCallback callback, void *argument) {
    if (!PMi_Lock()) return 1;
    data_02064cdc.callback = callback;
    data_02064cdc.argument = argument;
    PMi_SendPxiData((u8)(command >> 16) | 0x02006300);
    PMi_SendPxiData((command & 0xFFFF) | 0x01010000);
    return 0;
}

