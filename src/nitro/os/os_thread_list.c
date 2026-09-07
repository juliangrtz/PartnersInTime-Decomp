#include <nitro/os_sync.h>

int func_02039908(void) {
    int id;
    for (id = 0; id < 16; id++) {
        if (!OSi_ThreadInfo.threads[id]) return id;
    }
    return -1;
}

void OSi_InsertThreadToList(OsThread *thread) {
    OsThread *current = OSi_ThreadInfo.head;
    OsThread *previous = 0;
    while (current && current->priority < thread->priority) {
        previous = current;
        current = current->next;
    }
    if (!previous) {
        thread->next = OSi_ThreadInfo.head;
        OSi_ThreadInfo.head = thread;
    } else {
        thread->next = previous->next;
        previous->next = thread;
    }
}

void func_02039858(OsThread *thread) {
    OsThread *current = OSi_ThreadInfo.head;
    OsThread *previous = 0;
    while (current && current != thread) {
        previous = current;
        current = current->next;
    }
    if (!previous) OSi_ThreadInfo.head = thread->next;
    else previous->next = thread->next;
}
