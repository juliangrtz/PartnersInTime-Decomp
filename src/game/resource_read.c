#include <nitro/fs.h>
#include <game/heap.h>
extern u8 data_02054f68[];
void *GameResource_ReadFile(const char *path) {
    FsFile file;
    void *buffer;
    int size;
    FS_InitFile(&file);
    if (!FS_OpenFile(&file, path)) return 0;
    size = file.property.file.bottom - file.property.file.top;
    buffer = GameHeap_Allocate(1, (size + 3) & ~3, data_02054f68, 0);
    if (buffer) {
        if (size != FS_ReadFile(&file, buffer, size)) {
            GameHeap_Free(buffer);
            buffer = 0;
        }
    }
    FS_CloseFile(&file);
    return buffer;
}
void GameResource_Move16(const s16 *source, s16 *destination, u32 bytes) {
    if (source == destination) return;
    bytes >>= 1;
    if (!bytes) return;
    if (destination < source) {
        while (bytes--) *destination++ = *source++;
    } else {
        destination += bytes;
        source += bytes;
        while (bytes--) *--destination = *--source;
    }
}
