#ifndef NITRO_OS_MESSAGE_H
#define NITRO_OS_MESSAGE_H
#include <nitro/os_sync.h>

typedef struct OsMessageQueue {
    OsThreadQueue send_waiters;
    OsThreadQueue receive_waiters;
    void **messages;
    int capacity;
    int first;
    int count;
} OsMessageQueue;

void func_02039d08(OsMessageQueue *queue, void **buffer, int capacity);
int func_02039c60(OsMessageQueue *queue, void *message, int flags);
int func_02039bb4(OsMessageQueue *queue, void **message, int flags);
#endif
