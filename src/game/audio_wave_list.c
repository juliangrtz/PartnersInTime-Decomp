#include <game/audio.h>

extern int func_0203cfec(SoundBank *bank, int instrument, int key, SoundInstrument *output);

void GameAudio_LoadWaveList(const GameAudioWaveList *list, int deferred, u32 bytes_per_frame)
{
    NNSSndArcBankInfo *bank_info;
    const u16 *instrument;
    u16 loaded;
    NNSSndArcWaveArcInfo *wave_info;
    SoundWaveArchive *wave;
    SoundBank *bank;
    s16 remaining;
    if (data_0205ade8.active) return;
    NNS_SndHeapLoadState(data_0205ad64, NNS_SndHeapGetCurrentLevel(data_0205ad64));
    loaded = 0;
    bank_info = NNS_SndArcGetBankInfo(data_0205ad54);
    wave_info = NNS_SndArcGetWaveArcInfo(bank_info->wave_archives[0]);
    wave = NNS_SndArcGetFileAddress(wave_info->file);
    bank = NNS_SndArcGetFileAddress(bank_info->file);
    remaining = list->count;
    instrument = list->instruments;
    for (; remaining > 0; --remaining, ++instrument) {
        if (*instrument) {
            SoundInstrument parameters;
            func_0203cfec(bank, *instrument, 0, &parameters);
            GameAudio_LoadWave(wave, parameters.parameters.values[0], wave_info->file,
                data_0205ad64, deferred);
            ++loaded;
        }
    }
    if (loaded) {
        func_0203d2d0(bank, 0, wave);
        if (deferred) {
            data_0205ad28 = bytes_per_frame;
            GameAudio_StartLoader(0, 0, 255, 0);
        }
    }
}
