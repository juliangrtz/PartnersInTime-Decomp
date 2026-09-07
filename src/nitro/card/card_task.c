#include <nitro/card.h>

void CARDi_SetTask(CardTask task) {
    CardWork *work = &cardi_common;
    u32 priority = work->priority;
    switch (priority) {
    case 32:
        priority = func_0203929c(OSi_ThreadInfo.current);
        if (priority) priority--;
        break;
    case 33:
        priority = func_0203929c(OSi_ThreadInfo.current);
        if (priority < 31) priority++;
        break;
    }
    OS_SetThreadPriority(&work->thread, priority);
    work->current_thread = &work->thread;
    work->task = task;
    work->flags |= 8;
    OS_WakeupThreadDirect(&work->thread);
}
