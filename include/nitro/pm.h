#ifndef NITRO_PM_H
#define NITRO_PM_H

#include <nitro/os_sync.h>

typedef void (*PmCallback)(u32 result, void *argument);
typedef struct PmSleepCallback {
    void (*callback)(void *);
    void *argument;
    struct PmSleepCallback *next;
} PmSleepCallback;
typedef struct PmWork {
    volatile u32 busy;
    PmCallback callback;
    void *argument;
    u32 *shutdown_result;
} PmWork;
typedef struct PmRegisterResult { volatile u16 ready; u16 padding; u16 *output; } PmRegisterResult;

extern PmWork data_02064cdc;
extern PmRegisterResult data_02064d00[4];
/* Alias of the output-pointer column, with an eight-byte row stride. */
extern u8 data_02064d04[];
extern PmSleepCallback *data_02064cd0;
extern PmSleepCallback *data_02064cd8;

typedef char PmWorkSizeCheck[(sizeof(PmWork) == 16) ? 1 : -1];
typedef char PmRegisterResultSizeCheck[(sizeof(PmRegisterResult) == 8) ? 1 : -1];
typedef char PmSleepCallbackSizeCheck[(sizeof(PmSleepCallback) == 12) ? 1 : -1];

int PMi_Lock(void);
u32 PMi_WaitBusy(void);
void PMi_SetSyncResult(u32 result, void *output);
void PMi_Complete(u32 result);
void PM_Init(void);
void PMi_FifoCallback(u32 tag, u32 data, int error);
int PMi_SendSleepStart(u16 triggers, u16 keys);
int PMi_SendUtilityCommand(u32 command, PmCallback callback, void *argument);
int PMi_ReadRegisterAsync(u32 index, u16 *output, PmCallback callback, void *argument);
int PMi_ReadRegister(u32 index, u16 *output);
int PM_SetLEDAsync(u32 pattern, PmCallback callback, void *argument);
int PM_SetLED(u32 pattern);
int PM_SetBackLightAsync(int target, int enabled, PmCallback callback, void *argument);
int PM_SetBackLight(int target, int enabled);
int PM_ForceToPowerOffAsync(PmCallback callback, void *argument);
int PM_ForceToPowerOff(void);
int PM_GetBackLight(int *top, int *bottom);
void PMi_SendPxiData(u32 data);
void PM_GoSleepMode(u32 triggers, u32 keys, u32 key_logic);
int PM_SetLCDPower(int enabled, u32 led, int skip_delay, int synchronous);
int PM_GetLCDPower(void);
void PMi_PrependList(PmSleepCallback **head, PmSleepCallback *callback);
void PMi_AppendList(PmSleepCallback **head, PmSleepCallback *callback);
void PMi_RemoveList(PmSleepCallback **head, PmSleepCallback *callback);
void PMi_ExecuteList(PmSleepCallback *callback);

#endif
