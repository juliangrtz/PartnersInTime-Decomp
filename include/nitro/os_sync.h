#ifndef NITRO_OS_SYNC_H
#define NITRO_OS_SYNC_H

#include <nitro.h>

typedef u16 OsThreadQueue;
typedef struct OsMutex OsMutex;
typedef struct OsContext {
    u32 cpsr;
    u32 registers[13];
    u32 sp;
    u32 lr;
    u32 pc;
    u32 supervisor_sp;
    u32 coprocessor[7];
} OsContext;

typedef struct OsThread {
    OsContext context;
    u32 state;
    struct OsThread *next;
    u32 id;
    u32 priority;
    u32 unknown74;
    OsMutex *waiting_mutex;
    OsMutex *mutex_head;
    OsMutex *mutex_tail;
    u32 *stack_bottom;
    u32 *stack_top;
    u32 stack_offset;
    OsThreadQueue join_waiters;
    u16 padding92;
} OsThread;

typedef void (*OsThreadSwitchCallback)(OsThread *previous, OsThread *next);

struct OsMutex {
    OsThreadQueue queue;
    u16 padding02;
    OsThread *owner;
    s32 count;
    OsMutex *previous;
    OsMutex *next;
};

typedef struct OsThreadInfo {
    u16 needs_reschedule;
    u16 thread_count;
    u16 reschedule_lock;
    u16 padding06;
    OsThread *current;
    OsThread *head;
    OsThreadSwitchCallback callback;
    OsThread *threads[16];
} OsThreadInfo;

typedef char OsContextSizeCheck[(sizeof(OsContext) == 0x64) ? 1 : -1];
typedef char OsThreadSizeCheck[(sizeof(OsThread) == 0x94) ? 1 : -1];
typedef char OsMutexSizeCheck[(sizeof(OsMutex) == 0x14) ? 1 : -1];
typedef char OsThreadInfoSizeCheck[(sizeof(OsThreadInfo) == 0x54) ? 1 : -1];

extern OsThreadInfo OSi_ThreadInfo;
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_WakeupThread(OsThreadQueue *queue);
extern void OS_SleepThread(OsThreadQueue *queue);

#endif
