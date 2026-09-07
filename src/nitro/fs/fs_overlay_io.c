#include <nitro/fs_overlay.h>

extern const void *fsi_digest_key_ptr;
extern u32 fsi_digest_key_len;
extern void DGT_Hash2CalcHmac(void *output, const void *data, u32 size, const void *key, u32 key_size);
extern void DC_InvalidateRange(const void *data, u32 size);
extern void IC_InvalidateRange(const void *data, u32 size);

u32 FSi_GetOverlayBinarySize(const FsOverlayInfo *overlay) {
    if (overlay->flags & 1) return overlay->compressed_size;
    return overlay->ram_size;
}

void FS_ClearOverlayImage(const FsOverlayInfo *overlay) {
    u8 *address;
    u32 ram_size = overlay->ram_size;
    u32 total;
    address = overlay->address;
    total = ram_size + overlay->bss_size;
    IC_InvalidateRange(address, total);
    DC_InvalidateRange(address, total);
    MI_CpuFill8(address + ram_size, 0, total - ram_size);
}

FsFileId FS_GetOverlayFileID(const FsOverlayInfo *overlay) {
    FsFileId id;
    id.archive = &fsi_arc_rom;
    id.id = overlay->file_id;
    return id;
}

int FSi_LoadOverlayInfoCore(FsOverlayInfo *overlay, int processor, u32 id,
    FsArchive *archive, u32 arm9_offset, u32 arm9_size, u32 arm7_offset, u32 arm7_size) {
    u32 position;
    u32 offset;
    u32 size;
    FsFileId file_id;
    FsFile file;
    if (!processor) {
        size = arm9_size;
        offset = arm9_offset;
    } else {
        size = arm7_size;
        offset = arm7_offset;
    }
    position = 32 * id;
    if (position >= size) return 0;
    FS_InitFile(&file);
    if (!FS_OpenFileDirect(&file, archive, offset + position, offset + size, -1)) return 0;
    if (FS_ReadFile(&file, overlay, 32) != 32) {
        FS_CloseFile(&file);
        return 0;
    }
    FS_CloseFile(&file);
    overlay->processor = processor;
    file_id = FS_GetOverlayFileID(overlay);
    if (!FS_OpenFileFast(&file, file_id)) return 0;
    overlay->file_offset = file.property.file.top;
    overlay->file_size = file.property.file.bottom - file.property.file.top;
    FS_CloseFile(&file);
    return 1;
}

int FS_LoadOverlayInfo(FsOverlayInfo *overlay_argument, int processor_argument, u32 id) {
    FsOverlayInfo *overlay;
    int processor;
    const FsRomTable *table;
    FsFileId file_id;
    FsFile file;
    /* Preserve the native conditional-load order and its literal pool. */
    asm {
        movs processor, r1
        mov overlay, r0
        ldreq table, =fsi_ovt9
        ldrne table, =fsi_ovt7
    }
    if (table->offset) {
        u32 offset = 32 * id;
        if (offset >= table->size) return 0;
        MI_CpuCopy8((void *)(table->offset + offset), overlay, 32);
        overlay->processor = processor;
        FS_InitFile(&file);
        file_id = FS_GetOverlayFileID(overlay);
        if (!FS_OpenFileFast(&file, file_id)) return 0;
        overlay->file_offset = file.property.file.top;
        overlay->file_size = file.property.file.bottom - file.property.file.top;
        FS_CloseFile(&file);
        return 1;
    } else {
        const FsRomTable *arm9 = (const FsRomTable *)0x027FFE50;
        const FsRomTable *arm7 = (const FsRomTable *)0x027FFE58;
        return FSi_LoadOverlayInfoCore(overlay, processor, id, &fsi_arc_rom,
            arm9->offset, arm9->size, arm7->offset, arm7->size);
    }
}

int FS_LoadOverlayImageAsync(FsOverlayInfo *overlay, FsFile *file) {
    FsFileId file_id;
    u32 size;
    FS_InitFile(file);
    file_id = FS_GetOverlayFileID(overlay);
    if (!FS_OpenFileFast(file, file_id)) return 0;
    size = FSi_GetOverlayBinarySize(overlay);
    FS_ClearOverlayImage(overlay);
    if (size == FS_ReadFileAsync(file, overlay->address, size)) return 1;
    FS_CloseFile(file);
    return 0;
}

int FS_LoadOverlayImage(FsOverlayInfo *overlay) {
    FsFileId file_id;
    FsFile file;
    u32 size;
    FS_InitFile(&file);
    file_id = FS_GetOverlayFileID(overlay);
    if (!FS_OpenFileFast(&file, file_id)) return 0;
    size = FSi_GetOverlayBinarySize(overlay);
    FS_ClearOverlayImage(overlay);
    if (size != FS_ReadFile(&file, overlay->address, size)) {
        FS_CloseFile(&file);
        return 0;
    }
    FS_CloseFile(&file);
    return 1;
}

int FSi_CompareDigest(const void *expected, const void *data, u32 size) {
    u32 index;
    u32 digest[5];
    u8 key[64];
    u32 *cursor;
    MI_CpuFill8(digest, 0, sizeof(digest));
    MI_CpuCopy8(fsi_digest_key_ptr, key, fsi_digest_key_len);
    DGT_Hash2CalcHmac(digest, data, size, key, fsi_digest_key_len);
    cursor = digest;
    index = 0;
    do {
        u32 actual = *cursor;
        u32 reference;
        /* Preserve the original indexed read instead of a second pointer induction. */
        asm { ldr reference, [expected, index] }
        if (actual != reference) break;
        index += 4;
        cursor++;
    } while (index < 20);
    return index == 20;
}
