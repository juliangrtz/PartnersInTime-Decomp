#include <nns/snd_archive.h>

u32 NNS_SndArcGetFileOffset(u32 file)
{
    NNSSndArcFat *fat = data_02061e34->fat;
    return file >= fat->count ? 0 : fat->entries[file].offset;
}

u32 NNS_SndArcGetFileSize(u32 file)
{
    NNSSndArcFat *fat = data_02061e34->fat;
    return file >= fat->count ? 0 : fat->entries[file].size;
}

int NNS_SndArcReadFile(u32 file, void *buffer, u32 size, u32 offset)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcFat *fat = archive->fat;
    NNSSndArcFatEntry *entry;
    u32 available;
    if (file >= fat->count) {
        return -1;
    }
    entry = &fat->entries[file];
    available = entry->size - offset;
    if (size > available) {
        size = available;
    }
    if (!FS_SeekFile(&archive->file, entry->offset + offset, 0)) {
        return -1;
    }
    return FS_ReadFile(&archive->file, buffer, size);
}

FsFileId NNS_SndArcGetFileID(void)
{
    return data_02061e34->file_id;
}

void *NNS_SndArcGetFileAddress(u32 file)
{
    NNSSndArcFat *fat = data_02061e34->fat;
    return file >= fat->count ? 0 : fat->entries[file].address;
}

void NNS_SndArcSetFileAddress(u32 file, void *address)
{
    data_02061e34->fat->entries[file].address = address;
}

void NNSi_SndArcDisposeInfo(void *memory, u32 size, u32 archive, u32 argument)
{
    ((NNSSndArc *)archive)->info = 0;
}

void NNSi_SndArcDisposeFat(void *memory, u32 size, u32 archive, u32 argument)
{
    ((NNSSndArc *)archive)->fat = 0;
}

void NNSi_SndArcDisposeSymbols(void *memory, u32 size, u32 archive, u32 argument)
{
    ((NNSSndArc *)archive)->symbols = 0;
}
