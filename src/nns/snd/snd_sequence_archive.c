/*
 * Sequence archive (ARM9 resident, 0x020340D4-0x02034114).
 *
 * Resolves an entry inside a sequence archive.
 */

#include <nns/snd_archive.h>

NNSSndSeqArcEntry *NNSi_SndSeqArcGetEntry(NNSSndSeqArcFile *archive, int index)
{
    NNSSndSeqArcEntry *entry;
    if (index < 0) return 0;
    if ((u32)index >= archive->count) return 0;
    entry = &archive->entries[index];
    if (entry->offset == 0xffffffff) return 0;
    return entry;
}
