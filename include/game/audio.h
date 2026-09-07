#ifndef PIT_GAME_AUDIO_H
#define PIT_GAME_AUDIO_H
#include <nns/snd_arc_stream.h>
#include <nns/snd_capture_effect.h>

typedef struct GameAudioFade { s32 volume, current, step, state; } GameAudioFade;
typedef struct GameAudioDelayedEffect { s16 sequence, volume; int frames; } GameAudioDelayedEffect;
typedef struct GameAudioMusic { int sequence; u8 bank; u8 reserved[3]; } GameAudioMusic;
typedef struct GameAudioMusicFiles { void *sequence, *bank, *waves[4]; } GameAudioMusicFiles;
typedef struct GameAudioLoadCommand GameAudioLoadCommand;
struct GameAudioLoadCommand {
    GameAudioLoadCommand *next;
    u8 *destination;
    u32 file, size, offset;
    void (*callback)(GameAudioLoadCommand *command);
    void *argument;
};
typedef struct GameAudioLoader {
    GameAudioLoadCommand command;
    GameAudioLoadCommand *next;
    int active;
    u32 offset;
    int sequence;
    u8 bank;
    u8 reserved[3];
    int mode;
    int start;
} GameAudioLoader;
typedef struct GameAudioQueueEnd {
    GameAudioLoadCommand *next;
    GameAudioLoadCommand **tail;
} GameAudioQueueEnd;

typedef char GameAudioLoadCommandSizeCheck[(sizeof(GameAudioLoadCommand) == 28) ? 1 : -1];
typedef char GameAudioLoaderSizeCheck[(sizeof(GameAudioLoader) == 56) ? 1 : -1];
typedef char GameAudioMusicFilesSizeCheck[(sizeof(GameAudioMusicFiles) == 24) ? 1 : -1];

extern u8 data_0205ad14;
extern u8 *data_0205ad18;
extern int data_0205ad1c;
extern NNSSndHeap *data_0205ad20;
extern NNSSndHandle data_0205ad24;
extern u32 data_0205ad28;
extern NNSSndHandle data_0205ad2c;
extern GameAudioLoadCommand *data_0205ad30;
extern u32 data_0205ad34;
extern NNSSndHeap *data_0205ad38;
extern u32 data_0205ad3c, data_0205ad40;
extern s16 data_0205ad44[2];
extern u32 data_0205ad48;
extern s16 data_0205ad4c[2];
extern int data_0205ad50, data_0205ad54, data_0205ad58, data_0205ad5c;
extern NNSSndStrmHandle data_0205ad60;
extern NNSSndHeap *data_0205ad64;
extern GameAudioMusic data_0205ad68;
extern GameAudioQueueEnd data_0205ad70;
extern NNSSndHandle data_0205ad78[4];
extern NNSSndHandle data_0205ad84;
extern GameAudioFade data_0205ad88;
extern GameAudioDelayedEffect data_0205ad98[4];
extern GameAudioMusicFiles data_0205adb8[2];
extern GameAudioLoader data_0205ade8;
extern SoundWaveArchive data_0205ae20;
extern NNSSndArc data_0205ae5c;

void GameAudio_UpdateMusicFade(void);
void GameAudio_PlayEffect(int sequence, int volume);
void GameAudio_QueueEffect(s16 sequence, int delay, s16 volume);
void GameAudio_UpdateDelayedEffects(void);
void GameAudio_ClearDelayedEffects(void);
int GameAudio_IsLoading(void);
int GameAudio_IsStreamPlaying(void);
void GameAudio_StopStream(void);
void GameAudio_StartStream(int stream);
void GameAudio_SetOutputEffect(int type);
void GameAudio_StopVoice(int sequence);
void GameAudio_PlayVoice(s16 sequence, int volume);
u32 GameAudio_GetEffectMask(void);
void GameAudio_StopEffects(void);
void GameAudio_StopEffect(int sequence);
int GameAudio_PlayEffectOnFreeHandle(int sequence, int volume);
void GameAudio_PlayEffectOnce(int sequence, int volume);
void GameAudio_PlayEffectDelayed(s16 sequence, int delay, s16 volume);
void GameAudio_ClearMusicBank(int bank);
u32 GameAudio_GetMusicBanks(void);
int GameAudio_GetMusicState(void);
void GameAudio_FadeMusic(int out, int frames);
void GameAudio_StopMusic(int frames);
void GameAudio_SetMusic(int sequence);
void GameAudio_LoadMusic(int sequence, u8 bank, u32 bytes_per_frame);
void GameAudio_PlayMusic(int sequence, u8 bank, u32 bytes_per_frame);
void GameAudio_ProcessLoading(void);
void GameAudio_Update(void);
void GameAudio_Destroy(void);
void GameAudio_Init(int load_symbols, u32 first_music_heap, u32 second_music_heap);

void GameAudio_StartLoadedMusic(int sequence, int bank);
void GameAudio_PrepareMusic(u8 bank, int sequence, int start, u32 bytes_per_frame);
void GameAudio_InitLoaderQueue(void);
void GameAudio_ProcessLoader(void);
void GameAudio_LoadCommonBank(void);
int GameAudio_LoadSequenceBank(NNSSndHeap *heap, int sequence, GameAudioMusicFiles *files);
void *GameAudio_LoadWaveArchive(u32 file, NNSSndHeap *heap, int force, int register_file, int deferred);
void GameAudio_StartLoader(u8 mode, int sequence, u8 bank, int start);
int GameAudio_StartReverb(int size, int volume);
void GameAudio_UncacheFile(void *memory, NNSSndArc *archive, u32 file);
void GameAudio_DisposeWave(void *memory, u32 size, u32 archive, u32 index);
void GameAudio_DisposeWaveHeader(void *memory, u32 size, u32 archive, u32 file);
void GameAudio_DisposeBank(void *memory, u32 size, u32 archive, u32 file);
void GameAudio_DisposeWaveArchive(void *memory, u32 size, u32 archive, u32 file);
void GameAudio_DisposeSequence(void *memory, u32 size, u32 archive, u32 file);
int GameAudio_ProcessLoadChunks(void);
void GameAudio_QueueRead(GameAudioLoadCommand *buffer, u32 size, u32 file, u32 offset,
                        void (*callback)(GameAudioLoadCommand *), void *argument);
void *GameAudio_AllocReadBuffer(NNSSndHeap *heap, u32 size,
                              NNSSndHeapDisposeCallback dispose, u32 user0, u32 user1);
u32 GameAudio_GetWaveSize(const SoundWaveArchive *archive, u32 index);
int GameAudio_LoadWave(SoundWaveArchive *archive, u32 index, u32 file, NNSSndHeap *heap, int deferred);
void GameAudio_LoadBankWaves(SoundWaveArchive *archive, SoundBank *bank, int slot, u32 file, NNSSndHeap *heap);
void GameAudio_InitWaveTable(GameAudioLoadCommand *command);
void GameAudio_LoadDeferredWaves(int sequence, int bank);
#endif
