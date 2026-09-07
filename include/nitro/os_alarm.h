#ifndef NITRO_OS_ALARM_H
#define NITRO_OS_ALARM_H

#include <nitro/os_sync.h>

typedef void (*OsAlarmCallback)(void *argument);
typedef struct OsAlarm {
    OsAlarmCallback callback;
    void *argument;
    u32 tag;
    u64 fire;
    struct OsAlarm *previous;
    struct OsAlarm *next;
    u64 period;
    u64 start;
} OsAlarm;

void OS_InitAlarm(void);
void OS_InitVAlarm(void);
int OS_IsAlarmAvailable(void);
void OS_CreateAlarm(OsAlarm *alarm);
void OS_SetAlarm(OsAlarm *alarm, u64 delay, OsAlarmCallback callback, void *argument);
void OS_CancelAlarm(OsAlarm *alarm);

#endif
