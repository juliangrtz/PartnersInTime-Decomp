#include <nitro/rtc.h>

extern u16 data_02064d20;
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void func_0203d3ac(void);
extern int func_0203d590(u32 tag, int processor);
extern void func_0203d5b8(u32 tag, void (*callback)(u32, u32, int));
extern int func_0203d4dc(u32 tag, u32 data, int error);

int RTCi_ReadTime(void) { return RTCi_SendCommand(0x12); }
int RTCi_WriteStatus2(void) { return RTCi_SendCommand(0x27); }
int RTCi_SendCommand(u16 command) {
    return func_0203d4dc(5, (command << 8) & 0x7F00, 0) >= 0;
}

void RTC_Init(void) {
    if (data_02064d20) return;
    data_02064d20 = 1;
    data_02064d24.busy = 0;
    data_02064d24.callback = 0;
    data_02064d24.alarm_callback = 0;
    data_02064d24.output = 0;
    data_02064d24.output2 = 0;
    func_0203d3ac();
    while (!func_0203d590(5, 1)) {}
    func_0203d5b8(5, RTCi_FifoCallback);
}

u32 RTC_GetTimeAsync(RtcTime *time, RtcCallback callback, void *argument) {
    u32 state = OS_DisableInterrupts();
    if (data_02064d24.busy) {
        OS_RestoreInterrupts(state);
        return 1;
    }
    data_02064d24.busy = 1;
    OS_RestoreInterrupts(state);
    data_02064d24.operation = 1;
    data_02064d24.secondary = 0;
    data_02064d24.output = time;
    data_02064d24.callback = callback;
    data_02064d24.argument = argument;
    if (RTCi_ReadTime()) return 0;
    return 3;
}

u32 RTC_GetTime(RtcTime *time) {
    data_02064d24.sync_result = RTC_GetTimeAsync(time, RTCi_SetSyncResult, 0);
    if (!data_02064d24.sync_result) RTCi_WaitBusy();
    return data_02064d24.sync_result;
}

