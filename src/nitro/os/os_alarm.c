#include <nitro/os_alarm.h>

typedef struct OsAlarmList {
    OsAlarm *head;
    OsAlarm *tail;
} OsAlarmList;

typedef char OsAlarmSizeCheck[(sizeof(OsAlarm) == 44) ? 1 : -1];
extern u16 data_02063000;
extern OsAlarmList data_02063004;
extern u8 SDK_AUTOLOAD_DTCM_START[];
extern u64 func_0203a718(void);
extern void func_0203a6fc(int timer);
extern void func_0203a8ec(void *argument);
extern void func_02038cf8(int timer, OsAlarmCallback callback, void *argument);
extern u32 OS_EnableIrqMask(u32 mask);
extern u32 OS_DisableIrqMask(u32 mask);
extern void OS_Terminate(void);
void func_0203acdc(OsAlarm *alarm);
void func_0203ab24(OsAlarm *alarm, u64 fire);

#define TIMER1_COUNT (*(vu16 *)0x04000104)
#define TIMER1_CONTROL (*(vu16 *)0x04000106)
#define IRQ_CHECK (*(vu32 *)((u32)SDK_AUTOLOAD_DTCM_START + 0x3FF8))

void func_0203acdc(OsAlarm *alarm) {
    u64 now = func_0203a718();
    s64 delay;
    u16 reload;
    TIMER1_CONTROL = 0;
    delay = alarm->fire - now;
    func_02038cf8(1, func_0203a8ec, 0);
    reload = 0;
    if (delay < 0) reload = 0xFFFE;
    else if (delay < 0x10000) reload = (u16)~delay;
    TIMER1_COUNT = reload;
    TIMER1_CONTROL = 0xC1;
    OS_EnableIrqMask(16);
}

void OS_InitAlarm(void) {
    if (data_02063000) return;
    data_02063000 = 1;
    func_0203a6fc(1);
    data_02063004.head = 0;
    data_02063004.tail = 0;
    OS_DisableIrqMask(16);
}

int OS_IsAlarmAvailable(void) { return data_02063000; }

void OS_CreateAlarm(OsAlarm *alarm) {
    alarm->callback = 0;
    alarm->tag = 0;
}

void func_0203ab24(OsAlarm *alarm, u64 fire) {
    OsAlarm *current;
    if (alarm->period) {
        u64 now = func_0203a718();
        fire = alarm->start;
        if (fire < now) {
            fire += alarm->period * ((now - fire) / alarm->period + 1);
        }
    }
    alarm->fire = fire;
    current = data_02063004.head;
    while (current) {
        if ((s64)(fire - current->fire) < 0) {
            alarm->previous = current->previous;
            current->previous = alarm;
            alarm->next = current;
            if (alarm->previous) alarm->previous->next = alarm;
            else {
                data_02063004.head = alarm;
                func_0203acdc(alarm);
            }
            return;
        }
        current = current->next;
    }
    alarm->next = 0;
    current = data_02063004.tail;
    data_02063004.tail = alarm;
    alarm->previous = current;
    if (current) current->next = alarm;
    else {
        data_02063004.tail = alarm;
        data_02063004.head = alarm;
        func_0203acdc(alarm);
    }
}

void OS_SetAlarm(OsAlarm *alarm, u64 delay, OsAlarmCallback callback, void *argument) {
    u32 state;
    if (!alarm || alarm->callback) OS_Terminate();
    state = OS_DisableInterrupts();
    alarm->period = 0;
    alarm->callback = callback;
    alarm->argument = argument;
    func_0203ab24(alarm, delay + func_0203a718());
    OS_RestoreInterrupts(state);
}

void OS_CancelAlarm(OsAlarm *alarm) {
    u32 state = OS_DisableInterrupts();
    OsAlarm *next;
    if (!alarm->callback) {
        OS_RestoreInterrupts(state);
        return;
    }
    next = alarm->next;
    if (!next) data_02063004.tail = alarm->previous;
    else next->previous = alarm->previous;
    if (alarm->previous) alarm->previous->next = next;
    else {
        data_02063004.head = next;
        if (next) func_0203acdc(next);
    }
    alarm->callback = 0;
    alarm->period = 0;
    OS_RestoreInterrupts(state);
}

void func_0203a8fc(void) {
    u64 now;
    OsAlarm *alarm;
    OsAlarm *next;
    OsAlarmCallback callback;
    TIMER1_CONTROL = 0;
    OS_DisableIrqMask(16);
    IRQ_CHECK |= 16;
    now = func_0203a718();
    alarm = data_02063004.head;
    if (!alarm) return;
    if (now < alarm->fire) {
        func_0203acdc(alarm);
        return;
    }
    next = alarm->next;
    data_02063004.head = next;
    if (!next) data_02063004.tail = 0;
    else next->previous = 0;
    callback = alarm->callback;
    if (!alarm->period) alarm->callback = 0;
    if (callback) callback(alarm->argument);
    if (alarm->period) {
        alarm->callback = callback;
        func_0203ab24(alarm, 0);
    }
    if (data_02063004.head) func_0203acdc(data_02063004.head);
}
