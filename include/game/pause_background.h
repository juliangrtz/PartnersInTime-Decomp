#ifndef PIT_GAME_PAUSE_BACKGROUND_H
#define PIT_GAME_PAUSE_BACKGROUND_H
#include <nitro.h>
/* Background jobs share the pause-menu task allocation and parent counters. */
typedef struct PauseBackgroundTask {
    u8 unknown_00[16];
    struct PauseBackgroundTask *parent;
    u8 unknown_14[8];
    void *buffer;
    int state, counter, selection;
    u8 unknown_2c[16];
    int pending;
    u8 unknown_40[8];
} PauseBackgroundTask;
typedef char PauseBackgroundTask_SizeCheck[sizeof(PauseBackgroundTask) == 72 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
PauseBackgroundTask *PauseBackground_QueueBankSwap(int alternate);
void PauseBackground_ApplyBankSwap(PauseBackgroundTask *task);
PauseBackgroundTask *PauseBackground_QueueUpload(int alternate);
void PauseBackground_ApplyUpload(PauseBackgroundTask *task);
void PauseBackground_SelectBank(int alternate);
void PauseBackground_Upload(int alternate);
void PauseBackground_StartAlternateLoad(PauseBackgroundTask *parent, int selection, int buffered);
void PauseBackground_StartPageLoad(PauseBackgroundTask *parent);
void PauseBackground_LoadBuffered(PauseBackgroundTask *task);
void PauseBackground_LoadDirect(PauseBackgroundTask *task);
void PauseBackground_LoadPage(PauseBackgroundTask *task);
PauseBackgroundTask *PauseBackground_QueuePalette(void *palette, int bank);
void PauseBackground_LoadPalette(PauseBackgroundTask *task);
#ifdef __cplusplus
}
#endif
#endif
