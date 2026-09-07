#include <nitro/os_sync.h>

extern int data_02062d04;
extern OsThread **data_02062d08;
extern OsThread *data_02062d28[];
extern OsThread OSi_IdleThread;
extern OsThreadSwitchCallback OSi_SystemCallbackInSwitchThread;
extern u32 func_0203ade8(void);
extern void OS_Halt(void);
extern int OS_GetProcMode(void);
extern int OS_SaveContext(OsContext *context);
extern void OS_LoadContext(OsContext *context);
extern void OS_Terminate(void);
extern void func_02039e0c(OsThread *thread);
extern void func_02039858(OsThread *thread);
extern void OSi_InsertThreadToList(OsThread *thread);
extern int func_02039908(void);
extern void func_02039938(OsContext *context, u32 entry, u32 stack);
extern void func_0203b7a0(u32 value, void *destination, u32 size);
void func_02039574(void);
void OSi_RescheduleThread(void);
OsThread *OS_SelectThread(void);

void func_020395d0(OsThread *thread, void (*entry)(void *), void *argument,
                   void *stack_top, u32 stack_size, u32 priority) {
    u32 state = OS_DisableInterrupts();
    int id = func_02039908();
    u32 *stack_bottom;
    if (id < 0) {
        OS_RestoreInterrupts(state);
        return;
    }
    thread->priority = priority;
    thread->id = id;
    thread->state = 0;
    thread->unknown74 = 0;
    data_02062d28[id] = thread;
    OSi_InsertThreadToList(thread);
    stack_bottom = (u32 *)((u8 *)stack_top - stack_size);
    thread->stack_top = stack_top;
    thread->stack_bottom = stack_bottom;
    thread->stack_offset = 0;
    thread->stack_top[-1] = 0xFDDB597D;
    thread->stack_bottom[0] = 0x7BF9DD5B;
    thread->join_waiters = 0;
    func_02039938(&thread->context, (u32)entry, (u32)stack_top - 4);
    thread->context.registers[0] = (u32)argument;
    thread->context.lr = (u32)func_02039574;
    {
        volatile u32 zero = 0;
        func_0203b7a0(zero, stack_bottom + 1, stack_size - 8);
    }
    thread->waiting_mutex = 0;
    thread->mutex_head = 0;
    thread->mutex_tail = 0;
    OS_RestoreInterrupts(state);
}

void func_02039574(void) {
    OsThread *thread;
    OS_DisableInterrupts();
    thread = *data_02062d08;
    func_02039e0c(thread);
    func_02039858(thread);
    data_02062d28[thread->id] = 0;
    thread->state = 2;
    OS_WakeupThread(&thread->join_waiters);
    OSi_RescheduleThread();
    OS_Terminate();
}

void OS_SleepThread(OsThreadQueue *queue) {
    u32 state = OS_DisableInterrupts();
    OsThread *thread = *data_02062d08;
    if (queue) *queue |= (u16)(1 << thread->id);
    thread->state = 0;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(state);
}

void OS_WakeupThread(OsThreadQueue *queue) {
    u32 state = OS_DisableInterrupts();
    u32 waiters = *queue;
    if (waiters) {
        OsThread *thread = OSi_ThreadInfo.head;
        while (thread) {
            if (waiters & (1 << thread->id)) thread->state = 1;
            thread = thread->next;
        }
        *queue = 0;
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(state);
}

void OS_WakeupThreadDirect(OsThread *thread) {
    u32 state = OS_DisableInterrupts();
    thread->state = 1;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(state);
}

OsThread *OS_SelectThread(void) {
    OsThread *thread = OSi_ThreadInfo.head;
    while (thread && thread->state != 1) thread = thread->next;
    return thread;
}

void OSi_RescheduleThread(void) {
    OsThreadInfo *info = &OSi_ThreadInfo;
    OsThread *previous, *next;
    if (data_02062d04) return;
    if (info->reschedule_lock || OS_GetProcMode() == 0x12) {
        info->needs_reschedule = 1;
        return;
    }
    previous = *data_02062d08;
    next = OS_SelectThread();
    if (previous == next || !next) return;
    if (OS_SaveContext(&previous->context)) return;
    if (OSi_SystemCallbackInSwitchThread) {
        OSi_SystemCallbackInSwitchThread(previous, next);
    }
    if (info->callback) info->callback(previous, next);
    OSi_ThreadInfo.current = next;
    OS_LoadContext(&next->context);
}

int OS_SetThreadPriority(OsThread *thread, u32 priority) {
    OsThread *current = OSi_ThreadInfo.head;
    OsThread *previous = 0;
    u32 state = OS_DisableInterrupts();
    while (current && current != thread) {
        previous = current;
        current = current->next;
    }
    if (!current || current == &OSi_IdleThread) {
        OS_RestoreInterrupts(state);
        return 0;
    }
    if (current->priority != priority) {
        if (!previous) OSi_ThreadInfo.head = thread->next;
        else previous->next = thread->next;
        thread->priority = priority;
        OSi_InsertThreadToList(thread);
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(state);
    return 1;
}

u32 func_0203929c(const OsThread *thread) { return thread->priority; }

OsThreadSwitchCallback func_02039268(OsThreadSwitchCallback callback) {
    u32 state = OS_DisableInterrupts();
    OsThreadSwitchCallback previous = OSi_ThreadInfo.callback;
    OSi_ThreadInfo.callback = callback;
    OS_RestoreInterrupts(state);
    return previous;
}

void func_02039254(void) {
    func_0203ade8();
    for (;;) OS_Halt();
}

void func_02039250(void) {}
