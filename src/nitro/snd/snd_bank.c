#include <nitro/snd_bank.h>

extern const u8 data_0204fc34[724];
extern void DC_StoreRange(const void *data, u32 size);

void func_0203d2d0(SoundBank *bank, int index, SoundWaveArchive *archive) {
    func_0203c2ac();
    if (bank->waves[index].archive) {
        SoundWaveLink *previous;
        SoundWaveLink *link;
        if (archive == bank->waves[index].archive) {
            func_0203c298();
            return;
        }
        previous = bank->waves[index].archive->links;
        link = &bank->waves[index];
        if (link == previous) {
            bank->waves[index].archive->links = bank->waves[index].next;
            DC_StoreRange(bank->waves[index].archive, 60);
        } else {
            while (previous) {
                if (link == previous->next) break;
                previous = previous->next;
            }
            previous->next = bank->waves[index].next;
            DC_StoreRange(previous, sizeof(*previous));
        }
    }
    {
        SoundWaveLink *next = archive->links;
        archive->links = &bank->waves[index];
        bank->waves[index].next = next;
    }
    bank->waves[index].archive = archive;
    func_0203c298();
    DC_StoreRange(bank, 60);
    DC_StoreRange(archive, 60);
}

void func_0203d230(SoundBank *bank) {
    int index;
    SoundWaveLink *link;
    func_0203c2ac();
    link = bank->waves;
    for (index = 0; index < 4; index++, link++) {
        SoundWaveArchive *archive = bank->waves[index].archive;
        if (archive) {
            SoundWaveLink *previous = archive->links;
            if (link == previous) {
                archive->links = bank->waves[index].next;
                DC_StoreRange(archive, 60);
            } else {
                while (previous) {
                    if (link == previous->next) break;
                    previous = previous->next;
                }
                previous->next = bank->waves[index].next;
                DC_StoreRange(previous, sizeof(*previous));
            }
        }
    }
    func_0203c298();
}

void func_0203d1e4(SoundWaveArchive *archive) {
    SoundWaveLink *link;
    func_0203c2ac();
    link = archive->links;
    while (link) {
        SoundWaveLink *next = link->next;
        link->archive = 0;
        link->next = 0;
        DC_StoreRange(link, sizeof(*link));
        link = next;
    }
    func_0203c298();
}

int func_0203cfec(SoundBank *bank, int instrument, int key, SoundInstrument *output) {
    u32 entry;
    if (instrument < 0) return 0;
    func_0203c2ac();
    if ((u32)instrument >= bank->count) {
        func_0203c298();
        return 0;
    }
    entry = bank->instruments[instrument];
    output->type = entry;
    switch (output->type) {
    case 1: case 2: case 3: case 4: case 5:
        output->parameters = *(SoundInstrumentParameters *)((u8 *)bank + (entry >> 8));
        break;
    case 16: {
        SoundInstrumentRange *range = (SoundInstrumentRange *)((u8 *)bank + (entry >> 8));
        int last = range->last;
        int first = range->first;
        if (key < first || key > last) {
            func_0203c298();
            return 0;
        }
        *output = range->instruments[key - first];
        break;
    }
    case 17: {
        int index = 0;
        SoundInstrumentRegions *regions = (SoundInstrumentRegions *)((u8 *)bank + (entry >> 8));
        while (key > regions->last[index]) {
            if (++index >= 8) {
                func_0203c298();
                return 0;
            }
        }
        *output = regions->instruments[index];
        break;
    }
    /* Explicit unsupported types retain the original dense dispatch table. */
    case 0: case 6: case 7: case 8: case 9: case 10:
    case 11: case 12: case 13: case 14: case 15: default:
        func_0203c298();
        return 0;
    }
    func_0203c298();
    return 1;
}

SoundBankCursor func_0203cfcc(void) {
    SoundBankCursor cursor;
    cursor.instrument = 0;
    cursor.region = 0;
    return cursor;
}

int func_0203cdf4(SoundBank *bank, SoundInstrument *output, SoundBankCursor *cursor) {
    while (cursor->instrument < bank->count) {
        u32 entry = bank->instruments[cursor->instrument];
        output->type = entry;
        switch (output->type) {
        case 1: case 2: case 3: case 4: case 5:
            output->parameters = *(SoundInstrumentParameters *)((u8 *)bank + (entry >> 8));
            cursor->instrument++;
            return 1;
        case 16: {
            SoundInstrumentRange *range = (SoundInstrumentRange *)((u8 *)bank + (entry >> 8));
            while (cursor->region < (u32)(range->last - range->first + 1)) {
                *output = range->instruments[cursor->region];
                cursor->region++;
                return 1;
            }
            break;
        }
        case 17: {
            SoundInstrumentRegions *regions = (SoundInstrumentRegions *)((u8 *)bank + (entry >> 8));
            while (cursor->region < 8) {
                if (!regions->last[cursor->region]) break;
                *output = regions->instruments[cursor->region];
                cursor->region++;
                return 1;
            }
            break;
        }
        }
        cursor->instrument++;
        cursor->region = 0;
    }
    return 0;
}

u32 func_0203cdec(const SoundWaveArchive *archive) { return archive->count; }

void func_0203cdb4(SoundWaveArchive *archive, int index, void *wave) {
    func_0203c2ac();
    archive->offsets[index] = (u32)wave;
    DC_StoreRange(&archive->offsets[index], 4);
    func_0203c298();
}

void *func_0203cd6c(SoundWaveArchive *archive, int index) {
    void *wave;
    func_0203c2ac();
    wave = (void *)archive->offsets[index];
    if (wave) {
        if ((u32)wave < 0x02000000) wave = (u8 *)archive + (u32)wave;
    } else {
        wave = 0;
    }
    func_0203c298();
    return wave;
}

u16 func_0203ccf8(int attenuation) {
    u32 shift;
    u32 volume;
    if (attenuation < -723) attenuation = -723;
    else if (attenuation > 0) attenuation = 0;
    volume = data_0204fc34[attenuation + 723];
    if (attenuation < -240) shift = 3;
    else if (attenuation < -120) shift = 2;
    else if (attenuation < -60) shift = 1;
    else shift = 0;
    return volume | (shift << 8);
}
