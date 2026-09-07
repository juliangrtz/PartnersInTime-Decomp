#ifndef NITRO_SND_BANK_H
#define NITRO_SND_BANK_H

#include <nitro/snd_command.h>

typedef struct SoundWaveArchive SoundWaveArchive;
typedef struct SoundWaveLink {
    SoundWaveArchive *archive;
    struct SoundWaveLink *next;
} SoundWaveLink;

struct SoundWaveArchive {
    u8 unknown00[24];
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

typedef char SoundWaveLinkSizeCheck[(sizeof(SoundWaveLink) == 8) ? 1 : -1];
typedef char SoundBankSizeCheck[(sizeof(SoundBank) == 64) ? 1 : -1];
typedef char SoundWaveArchiveSizeCheck[(sizeof(SoundWaveArchive) == 64) ? 1 : -1];
typedef char SoundInstrumentSizeCheck[(sizeof(SoundInstrument) == 12) ? 1 : -1];

#endif
