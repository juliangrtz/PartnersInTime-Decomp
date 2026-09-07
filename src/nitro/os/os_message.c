#include <nitro/os_message.h>

void func_02039d08(OsMessageQueue *queue, void **buffer, int capacity) {
    queue->send_waiters = 0;
    queue->receive_waiters = 0;
    queue->messages = buffer;
    queue->capacity = capacity;
    queue->first = 0;
    queue->count = 0;
}

int func_02039c60(OsMessageQueue *queue, void *message, int flags) {
    u32 state = OS_DisableInterrupts();
    while (queue->capacity <= queue->count) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(state);
            return 0;
        }
        OS_SleepThread(&queue->send_waiters);
    }
    queue->messages[(queue->first + queue->count) % queue->capacity] = message;
    queue->count++;
    OS_WakeupThread(&queue->receive_waiters);
    OS_RestoreInterrupts(state);
    return 1;
}

int func_02039bb4(OsMessageQueue *queue, void **message, int flags) {
    u32 state = OS_DisableInterrupts();
    while (queue->count == 0) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(state);
            return 0;
        }
        OS_SleepThread(&queue->receive_waiters);
    }
    if (message) *message = queue->messages[queue->first];
    queue->first = (queue->first + 1) % queue->capacity;
    queue->count--;
    OS_WakeupThread(&queue->send_waiters);
    OS_RestoreInterrupts(state);
    return 1;
}
