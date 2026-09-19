/*
 * Sound archive waves (ARM9 resident, 0x02031354-0x02031424).
 *
 * Loads the wave archives a bank's instruments refer to.
 */

#include <nns/snd_archive.h>

int NNSi_SndArcLoadWaves(SoundWaveArchive *wave, SoundBank *bank, int slot, u32 file, NNSSndHeap *heap)
{
    SoundBankCursor cursor = func_0203cfcc();
    SoundInstrument instrument;
    if (!bank) return 0;
    while (func_0203cdf4(bank, &instrument, &cursor)) {
        if (instrument.type == 1 && slot == instrument.parameters.values[1]) {
            if (!NNSi_SndArcLoadWave(wave, instrument.parameters.values[0], file, heap)) return 0;
        }
    }
    return 1;
}
