#include <nns/snd_archive.h>

void NNS_SndArcInit(NNSSndArc *archive, const char *path, NNSSndHeap *heap, int load_symbols)
{
    archive->info = 0;
    archive->fat = 0;
    archive->symbols = 0;
    if (!FS_ConvertPathToFileID(&archive->file_id, path)) {
        return;
    }
    FS_InitFile(&archive->file);
    if (!FS_OpenFileFast(&archive->file, archive->file_id)) {
        return;
    }
    archive->file_open = 1;
    if (NNSi_SndArcLoadHeader(archive, heap, load_symbols)) {
        data_02061e34 = archive;
    }
}

int NNSi_SndArcLoadHeader(NNSSndArc *archive, NNSSndHeap *heap, int load_symbols)
{
    int bytes_read;
    if (!FS_SeekFile(&archive->file, 0, 0)) {
        return 0;
    }
    if (FS_ReadFile(&archive->file, &archive->header, sizeof(NNSSndArcHeader)) != sizeof(NNSSndArcHeader)) {
        return 0;
    }
    if (heap) {
        archive->info = NNS_SndHeapAlloc(heap, archive->header.info_size, NNSi_SndArcDisposeInfo, (u32)archive, 0);
        if (!archive->info) {
            return 0;
        }
        if (!FS_SeekFile(&archive->file, archive->header.info_offset, 0)) {
            return 0;
        }
        bytes_read = FS_ReadFile(&archive->file, archive->info, archive->header.info_size);
        if (bytes_read != archive->header.info_size) {
            return 0;
        }
        archive->fat = NNS_SndHeapAlloc(heap, archive->header.fat_size, NNSi_SndArcDisposeFat, (u32)archive, 0);
        if (!archive->fat) {
            return 0;
        }
        if (!FS_SeekFile(&archive->file, archive->header.fat_offset, 0)) {
            return 0;
        }
        bytes_read = FS_ReadFile(&archive->file, archive->fat, archive->header.fat_size);
        if (bytes_read != archive->header.fat_size) {
            return 0;
        }
        if (load_symbols && archive->header.symbol_size) {
            archive->symbols = NNS_SndHeapAlloc(heap, archive->header.symbol_size, NNSi_SndArcDisposeSymbols, (u32)archive, 0);
            if (!archive->symbols) {
                return 0;
            }
            if (!FS_SeekFile(&archive->file, archive->header.symbol_offset, 0)) {
                return 0;
            }
            bytes_read = FS_ReadFile(&archive->file, archive->symbols, archive->header.symbol_size);
            if (bytes_read != archive->header.symbol_size) {
                return 0;
            }
        }
    }
    return 1;
}

NNSSndArc *NNS_SndArcSetCurrent(NNSSndArc *archive)
{
    NNSSndArc *previous = data_02061e34;
    data_02061e34 = archive;
    return previous;
}

NNSSndArc *NNS_SndArcGetCurrent(void)
{
    return data_02061e34;
}
