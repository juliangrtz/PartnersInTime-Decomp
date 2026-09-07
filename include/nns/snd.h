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
typedef struct NNSSndHeap NNSSndHeap;
typedef void (*NNSSndHeapDisposeCallback)(void *memory, u32 size, u32 user0, u32 user1);

typedef struct NNSiSndHeapBlock {
    NNSFndLink link;
    u32 size;
    NNSSndHeapDisposeCallback dispose;
    u32 user0;
    u32 user1;
    u32 reserved18[2];
} NNSiSndHeapBlock;

typedef struct NNSiSndHeapSection {
    NNSFndList blocks;
    NNSFndLink link;
} NNSiSndHeapSection;

struct NNSSndHeap {
    NNSFndHeapHead *frame;
    NNSFndList sections;
};

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
    NNSSndHeap *heap;
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
    u16 sequence_kind;
    u16 unknown36;
    u16 archive_or_sequence;
    u16 archive_sequence;
    u8 index;
    u8 priority;
    s16 volume_db;
    u8 initial_volume;
    u8 external_volume;
    u8 padding42[2];
};

typedef char NNSSndHeapSizeCheck[(sizeof(NNSSndHeap) == 16) ? 1 : -1];
typedef char NNSiSndHeapSectionSizeCheck[(sizeof(NNSiSndHeapSection) == 20) ? 1 : -1];
typedef char NNSiSndHeapBlockSizeCheck[(sizeof(NNSiSndHeapBlock) == 32) ? 1 : -1];
typedef char NNSiSndPlayerSizeCheck[(sizeof(NNSiSndPlayer) == 36) ? 1 : -1];
typedef char NNSiSndSeqPlayerSizeCheck[(sizeof(NNSiSndSeqPlayer) == 68) ? 1 : -1];

void NNSi_SndFaderInit(NNSiSndFader *fader);
void NNSi_SndFaderSet(NNSiSndFader *fader, int target, int duration);
int NNSi_SndFaderGet(NNSiSndFader *fader);
void NNSi_SndFaderUpdate(NNSiSndFader *fader);
int NNSi_SndFaderIsFinished(NNSiSndFader *fader);

void NNS_SndInit(void);
void NNS_SndMain(void);
void NNSi_SndInitResource(void);
void NNSi_SndFreeAlarm(int alarm);
int NNSi_SndAllocAlarm(void);
void NNSi_SndFreeCapture(u32 mask);
int NNSi_SndAllocCapture(u32 mask);
void NNSi_SndUnlockChannel(u32 mask);
int NNSi_SndLockChannel(u32 mask);

void NNSi_SndPlayerSetPriority(NNSiSndSeqPlayer *sequence, u8 priority);
void NNSi_SndPlayerDisposeHeap(void *memory, u32 size, u32 user0, u32 user1);
void NNSi_SndPlayerFreeSequence(NNSiSndSeqPlayer *sequence);
NNSiSndSeqPlayer *NNSi_SndPlayerAllocSequence(int priority);
void NNSi_SndPlayerForceStop(NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerInsertActive(NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerInsert(NNSiSndPlayer *player, NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerResetSequence(NNSiSndSeqPlayer *sequence);
void *NNSi_SndPlayerAllocHeap(int index, NNSiSndSeqPlayer *sequence);
void NNSi_SndPlayerStopSequence(NNSiSndSeqPlayer *sequence, int frames);
void NNSi_SndPlayerPrepareSequence(NNSiSndSeqPlayer *sequence, const void *data, u32 offset, const void *bank);
void NNSi_SndPlayerReleaseSequence(NNSiSndSeqPlayer *sequence);
NNSiSndSeqPlayer *NNSi_SndPlayerAlloc(NNSSndHandle *handle, int index, int priority);
void NNSi_SndPlayerMain(void);
void NNSi_SndPlayerInit(void);

void NNS_SndPlayerSetPlayableSeqCount(int player, int count);
void NNS_SndPlayerSetAllocatableChannel(int player, u32 mask);
int NNS_SndPlayerCreateHeap(int player, void *heap, u32 size);
void NNS_SndStopSeq(NNSSndHandle *handle, int frames);
void NNS_SndStopSeqArc(int archive, int frames);
void NNS_SndStopSeqArcIdx(int archive, int index, int frames);
void NNS_SndHandleInit(NNSSndHandle *handle);
void NNS_SndHandleReleaseSeq(NNSSndHandle *handle);
int NNS_SndCountPlayingSeqArcIdx(int archive, int index);
void NNSi_SndPlayerSetInitialVolume(NNSSndHandle *handle, u8 volume);
void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, int volume, int frames);
void NNS_SndPlayerSetChannelPriority(NNSSndHandle *handle, u32 priority);
void NNS_SndPlayerSetTrackVolume(NNSSndHandle *handle, u32 tracks, int volume);
void NNSi_SndPlayerSetSeqNo(NNSSndHandle *handle, int sequence);
void NNSi_SndPlayerSetSeqArcNo(NNSSndHandle *handle, int archive, int sequence);
int NNS_SndPlayerWriteGlobalVariable(u32 index, s16 value);

void NNSi_SndHeapWaitCommands(void);
int NNSi_SndHeapNewSection(NNSSndHeap *heap);
int NNSi_SndHeapInit(NNSSndHeap *heap, NNSFndHeapHead *frame);
void NNSi_SndHeapInitSection(NNSiSndHeapSection *section);
int NNS_SndHeapGetCurrentLevel(NNSSndHeap *heap);
void NNS_SndHeapLoadState(NNSSndHeap *heap, int level);
int NNS_SndHeapSaveState(NNSSndHeap *heap);
void *NNS_SndHeapAlloc(NNSSndHeap *heap, u32 size, NNSSndHeapDisposeCallback dispose, u32 user0, u32 user1);
void NNS_SndHeapClear(NNSSndHeap *heap);
void NNS_SndHeapDestroy(NNSSndHeap *heap);
NNSSndHeap *NNS_SndHeapCreate(void *start, u32 size);

#endif
