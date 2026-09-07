#include <nitro/pm.h>

extern u16 data_02064cc4;
extern volatile u32 data_02064cc8;
extern volatile u32 data_02064ccc;
extern u32 data_02064cd4;
extern OsMutex data_02064cec;
extern void func_0203d3ac(void);
extern int func_0203d590(u32 tag, int processor);
extern void func_0203d5b8(u32 tag, void (*callback)(u32, u32, int));
extern int func_0203d4dc(u32 tag, u32 data, int error);
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

int PMi_ReadRegister(u32 index, u16 *output) {
    u32 result;
    int status = PMi_ReadRegisterAsync(index, output, PMi_SetSyncResult, &result);
    if (status) return status;
    PMi_WaitBusy();
    return result;
}

int PM_SetLEDAsync(u32 pattern, PmCallback callback, void *argument) {
    u32 command;
    switch (pattern) {
    case 1: command = 1; break;
    case 3: command = 2; break;
    case 2: command = 3; break;
    default: command = 0; break;
    }
    if (!command) return 0xFFFF;
    return PMi_SendUtilityCommand(command, callback, argument);
}
int PM_SetLED(u32 pattern) {
    u32 result;
    int status = PM_SetLEDAsync(pattern, PMi_SetSyncResult, &result);
    if (status) return status;
    PMi_WaitBusy();
    return result;
}

int PM_SetBackLightAsync(int target, int enabled, PmCallback callback, void *argument) {
    u32 command = 0;
    if (target == 0) {
        if (enabled == 1) command = 6;
        if (enabled == 0) command = 7;
    } else if (target == 1) {
        if (enabled == 1) command = 4;
        if (enabled == 0) command = 5;
    } else if (target == 2) {
        if (enabled == 1) command = 8;
        if (enabled == 0) command = 9;
    }
    if (!command) return 0xFFFF;
    return PMi_SendUtilityCommand(command, callback, argument);
}
int PM_SetBackLight(int target, int enabled) {
    u32 result;
    int status = PM_SetBackLightAsync(target, enabled, PMi_SetSyncResult, &result);
    if (status) return status;
    PMi_WaitBusy();
    return result;
}

int PM_ForceToPowerOffAsync(PmCallback callback, void *argument) {
    return PMi_SendUtilityCommand(14, callback, argument);
}
int PM_ForceToPowerOff(void) {
    u32 result;
    int status = PM_ForceToPowerOffAsync(PMi_SetSyncResult, &result);
    if (status) return status;
    PMi_WaitBusy();
    return result;
}

int PM_GetBackLight(int *top, int *bottom) {
    u16 value;
    int result = PMi_ReadRegister(0, &value);
    if (!result) {
        if (top) *top = (value & 8) != 0;
        if (bottom) *bottom = (value & 4) != 0;
    }
    return result;
}

void PMi_SendPxiData(u32 data) { while (func_0203d4dc(8, data, 0) != 0) {} }

void PM_GoSleepMode(u32 triggers, u32 keys, u32 key_logic) {
    int power_off = 0;
    volatile u32 state;
    volatile u32 irq_mask;
    u32 main_display, sub_display;
    volatile int lcd;
    int top, bottom;
    volatile u32 frame;
    u16 sleep_triggers;
    u16 master;
    PMi_ExecuteList(data_02064cd0);
    master = PM_IME;
    PM_IME = 0;
    state = OS_DisableInterrupts();
    irq_mask = OS_DisableIrqMask(0x3FFFFF);
    func_02038cc4(0x40000);
    OS_RestoreInterrupts(state);
    (void)PM_IME;
    PM_IME = 1;
    if ((triggers & 8) && *(vu16 *)0x027FFC40 == 2) triggers &= ~8;
    if ((triggers & 16) && !CTRDG_IsExisting(triggers & 16)) triggers &= ~16;
    main_display = *(vu32 *)0x04000000;
    sub_display = *(vu32 *)0x04001000;
    lcd = PM_GetLCDPower();
    PM_GetBackLight(&top, &bottom);
    PM_SetBackLight(2, 0);
    frame = PM_FRAME;
    while (frame == PMi_GetFrame()) {}
    frame = PM_FRAME;
    GX_DispOff();
    *(vu32 *)0x04001000 &= ~0x10000;
    while (frame == PMi_GetFrame()) {}
    frame = PM_FRAME;
    while (frame == PMi_GetFrame()) {}
    sleep_triggers = triggers | (top << 5) | (bottom << 6);
    keys |= key_logic;
    while (PMi_SendSleepStart(sleep_triggers, keys)) {}
    OS_Halt();
    func_0203ae6c(0x332340);
    if ((triggers & 8) && (*(vu32 *)0x04000214 & 0x100000)) power_off = 1;
    if (!power_off) {
        if (lcd == 1) {
            PM_SetLCDPower(1, 1, 1, 0);
            GX_DispOn();
        } else {
            PM_SetLED(1);
        }
        *(vu32 *)0x04000000 = main_display;
        *(vu32 *)0x04001000 = sub_display;
    }
    OS_DisableInterrupts();
    func_02038cc4(irq_mask);
    OS_RestoreInterrupts(state);
    (void)PM_IME;
    PM_IME = master;
    if (power_off) PM_ForceToPowerOff();
    PMi_ExecuteList(data_02064cd8);
}

int PM_SetLCDPower(int enabled, u32 led, int skip_delay, int synchronous) {
    if (enabled) {
        if (enabled == 1) {
            if (!skip_delay && PM_FRAME - data_02064cd4 <= 7) return 0;
            if (led) {
                if (synchronous) PM_SetLED(led);
                else PM_SetLEDAsync(led, 0, 0);
            }
            PM_POWER |= 1;
        }
    } else {
        PM_POWER &= ~1;
        data_02064cd4 = PM_FRAME;
        if (led) {
            if (synchronous) PM_SetLED(led);
            else PM_SetLEDAsync(led, 0, 0);
        }
    }
    return 1;
}
int PM_GetLCDPower(void) { return (PM_POWER & 1) != 0; }

void PMi_PrependList(PmSleepCallback **head, PmSleepCallback *callback) {
    if (head) { callback->next = *head; *head = callback; }
}
void PMi_AppendList(PmSleepCallback **head, PmSleepCallback *callback) {
    PmSleepCallback *last;
    PmSleepCallback *next;
    if (!head) return;
    last = *head;
    if (!last) { callback->next = 0; *head = callback; return; }
    next = last->next;
    while (next) { last = next; next = next->next; }
    callback->next = next;
    last->next = callback;
}
void PMi_RemoveList(PmSleepCallback **head, PmSleepCallback *callback) {
    PmSleepCallback *current;
    PmSleepCallback *previous;
    if (!head) return;
    current = *head;
    previous = current;
    while (current) {
        if (current == callback) {
            if (current == previous) *head = current->next;
            else previous->next = current->next;
            return;
        }
        previous = current;
        current = current->next;
    }
}
void PMi_ExecuteList(PmSleepCallback *callback) {
    for (; callback; callback = callback->next) callback->callback(callback->argument);
}
void PM_PrependPreSleepCallback(PmSleepCallback *callback) { PMi_PrependList(&data_02064cd0, callback); }
void PM_AppendPostSleepCallback(PmSleepCallback *callback) { PMi_AppendList(&data_02064cd8, callback); }
void PM_DeletePreSleepCallback(PmSleepCallback *callback) { PMi_RemoveList(&data_02064cd0, callback); }
void PM_DeletePostSleepCallback(PmSleepCallback *callback) { PMi_RemoveList(&data_02064cd8, callback); }
