#ifndef NNS_SND_H
#define NNS_SND_H

#include <nns/fnd.h>

typedef struct NNSiSndFader {
    int origin;
    int target;
    int position;
    int duration;
} NNSiSndFader;

typedef struct NNSiSndSeqPlayer NNSiSndSeqPlayer;
typedef struct NNSiSndPlayer NNSiSndPlayer;
typedef struct NNSiSndSeqHeap NNSiSndSeqHeap;

typedef struct NNSSndHandle {
    NNSiSndSeqPlayer *player;
} NNSSndHandle;

struct NNSiSndPlayer {
    NNSFndList sequences;
    NNSFndList heaps;
    u32 max_sequences;
    u32 channel_mask;
    u8 volume;
    u8 padding21[3];
};

struct NNSiSndSeqHeap {
    NNSFndLink link;
    void *heap;
    NNSiSndSeqPlayer *player;
    int player_index;
};

struct NNSiSndSeqPlayer {
    NNSSndHandle *handle;
    NNSiSndPlayer *owner;
    NNSiSndSeqHeap *heap;
    NNSFndLink player_link;
    NNSFndLink global_link;
    NNSiSndFader fader;
    u8 state;
    u8 started;
    u8 paused;
    u8 start_pending;
    u32 command;
    u16 sequence;
    u8 unknown36[6];
    u8 index;
    u8 priority;
    s16 volume_db;
    u8 initial_volume;
    u8 external_volume;
    u8 padding42[2];
};

typedef char NNSiSndPlayerSizeCheck[(sizeof(NNSiSndPlayer) == 36) ? 1 : -1];
typedef char NNSiSndSeqPlayerSizeCheck[(sizeof(NNSiSndSeqPlayer) == 68) ? 1 : -1];

void NNSi_SndFaderInit(NNSiSndFader *fader);
void NNSi_SndFaderSet(NNSiSndFader *fader, int target, int duration);
int NNSi_SndFaderGet(NNSiSndFader *fader);
void NNSi_SndFaderUpdate(NNSiSndFader *fader);
int NNSi_SndFaderIsFinished(NNSiSndFader *fader);

void NNSi_SndInitResource(void);
void NNSi_SndFreeAlarm(int alarm);
int NNSi_SndAllocAlarm(void);
void NNSi_SndFreeCapture(u32 mask);
int NNSi_SndAllocCapture(u32 mask);
void NNSi_SndUnlockChannel(u32 mask);
int NNSi_SndLockChannel(u32 mask);

void NNSi_SndPlayerSetPriority(NNSiSndSeqPlayer *sequence, u8 priority);
void NNSi_SndPlayerDisposeHeap(NNSiSndSeqHeap *heap);
void NNSi_SndPlayerFreeSequence(NNSiSndSeqPlayer *sequence);
NNSiSndSeqPlayer *NNSi_SndPlayerAllocSequence(int priority);
void NNSi_SndPlayerForceStop(NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerInsertActive(NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerInsert(NNSiSndPlayer *player, NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerResetSequence(NNSiSndSeqPlayer *sequence);
void *NNSi_SndPlayerAllocHeap(int index, NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerStopSequence(NNSiSndSeqPlayer *sequence, int frames);
void NNSi_SndPlayerPrepareSequence(NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerReleaseSequence(NNSiSndSeqPlayer *sequence);
NNSiSndSeqPlayer *NNSi_SndPlayerAlloc(NNSSndHandle *handle, int index, int priority);
void NNSi_SndPlayerMain(void);
void NNSi_SndPlayerInit(void);

#endif
