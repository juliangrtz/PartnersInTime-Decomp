#include <nitro/card.h>

void CARDi_EndBackupTask(CardWork *work) {
    CardCallback callback = work->callback;
    void *argument = work->argument;
    u32 state = OS_DisableInterrupts();
    work->flags &= ~12;
    OS_WakeupThread(&work->task_queue);
    if (work->flags & 16) OS_WakeupThreadDirect(&work->thread);
    OS_RestoreInterrupts(state);
    if (callback) callback(argument);
}
