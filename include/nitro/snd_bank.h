#ifndef NITRO_SND_BANK_H
#define NITRO_SND_BANK_H

#include <nitro/snd_command.h>

typedef struct SoundWaveArchive SoundWaveArchive;
typedef struct SoundWaveLink {
    SoundWaveArchive *archive;
    struct SoundWaveLink *next;
} SoundWaveLink;

struct SoundWaveArchive {
    u8 unknown00[8];
    u32 file_size;
    u8 unknown0c[12];
    SoundWaveLink *links;
    u8 unknown1c[28];
    u32 count;
    u32 offsets[1];
};

typedef struct SoundBank {
    u8 unknown00[24];
    SoundWaveLink waves[4];
    u32 count;
    u32 instruments[1];
} SoundBank;

typedef struct SoundInstrumentParameters { u16 values[5]; } SoundInstrumentParameters;
typedef struct SoundInstrument {
    u8 type;
    u8 padding01;
    SoundInstrumentParameters parameters;
} SoundInstrument;
typedef struct SoundInstrumentRange {
    u8 first;
    u8 last;
    SoundInstrument instruments[1];
} SoundInstrumentRange;
typedef struct SoundInstrumentRegions {
    u8 last[8];
    SoundInstrument instruments[1];
} SoundInstrumentRegions;
typedef struct SoundBankCursor { u32 instrument; u32 region; } SoundBankCursor;

void *func_0203cd6c(SoundWaveArchive *archive, int index);
void func_0203cdb4(SoundWaveArchive *archive, int index, void *wave);
u32 func_0203cdec(const SoundWaveArchive *archive);
int func_0203cdf4(SoundBank *bank, SoundInstrument *output, SoundBankCursor *cursor);
SoundBankCursor func_0203cfcc(void);
void func_0203d1e4(SoundWaveArchive *archive);
void func_0203d230(SoundBank *bank);
void func_0203d2d0(SoundBank *bank, int index, SoundWaveArchive *archive);

typedef char SoundWaveLinkSizeCheck[(sizeof(SoundWaveLink) == 8) ? 1 : -1];
typedef char SoundBankSizeCheck[(sizeof(SoundBank) == 64) ? 1 : -1];
typedef char SoundWaveArchiveSizeCheck[(sizeof(SoundWaveArchive) == 64) ? 1 : -1];
typedef char SoundInstrumentSizeCheck[(sizeof(SoundInstrument) == 12) ? 1 : -1];

#endif
