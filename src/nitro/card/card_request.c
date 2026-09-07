#include <nitro/card.h>

int CARDi_Request(CardWork *work, u32 command, int retries) {
    u32 result;
    if (!(work->flags & 2)) {
        work->flags |= 2;
        while (!func_0203d590(11, 1)) func_0203ae6c(100);
        CARDi_Request(work, 0, 1);
    }
    DC_FlushRange(work->command, 64);
    DC_WaitWriteBufferEmpty();
    do {
        u32 state;
        work->reserved04 = command;
        work->flags |= 32;
        while (func_0203d4dc(11, command, 1) < 0) {}
        if (!command) {
            u32 address = (u32)work->command;
            while (func_0203d4dc(11, address, 1) < 0) {}
        }
        state = OS_DisableInterrupts();
        while (work->flags & 32) OS_SleepThread(0);
        OS_RestoreInterrupts(state);
        result = work->command->result;
        if (result != 4) break;
        retries--;
    } while (retries > 0);
    return result == 0;
}
