#ifndef NITRO_RTC_H
#define NITRO_RTC_H

#include <nitro.h>

typedef void (*RtcCallback)(u32 result, void *argument);
typedef struct RtcDate { int year, month, day, weekday; } RtcDate;
typedef struct RtcTime { int hour, minute, second; } RtcTime;
typedef struct RtcAlarm { int weekday, hour, minute, enabled; } RtcAlarm;
typedef struct RtcWork {
    volatile u32 busy;
    RtcCallback callback;
    void *output;
    void *output2;
    void *argument;
    u32 operation;
    u32 secondary;
    void (*alarm_callback)(void);
    u32 sync_result;
} RtcWork;

extern RtcWork data_02064d24;
typedef char RtcWorkSizeCheck[(sizeof(RtcWork) == 36) ? 1 : -1];

void RTCi_WaitBusy(void);
void RTCi_SetSyncResult(u32 result, void *argument);
u32 RTCi_ConvertBcdToBinary(u32 value);
void RTCi_FifoCallback(u32 tag, u32 data, int error);
u32 RTC_GetTime(RtcTime *time);
u32 RTC_GetTimeAsync(RtcTime *time, RtcCallback callback, void *argument);
void RTC_Init(void);
int RTCi_SendCommand(u16 command);
int RTCi_WriteStatus2(void);
int RTCi_ReadTime(void);

#endif
