#include <nitro/fs_overlay.h>

typedef struct FsDestructor {
    struct FsDestructor *next;
    void (*callback)(void *);
    void *argument;
} FsDestructor;

extern FsDestructor *__global_destructor_chain;
extern const u8 fsi_digest_table_start[];
extern const u8 fsi_digest_table_end[];
extern int FSi_CompareDigest(const void *expected, const void *data, u32 size);
extern void MIi_UncompressBackward(void *end);
extern void DC_FlushRange(const void *data, u32 size);
extern void OS_Terminate(void);

static inline u32 FSi_GetOverlayBssLength(u32 ram_size, u32 bss_size) {
    s32 total = (s32)(ram_size + bss_size);
    return total - (s32)ram_size;
}

void FS_StartOverlay(FsOverlayInfo *overlay) {
    u32 size = FSi_GetOverlayBinarySize(overlay);
    void (**initializer)(void);
    void (**end)(void);
    if (*(vu16 *)0x027FFC40 == 2) {
        int verified = 0;
        if (overlay->flags & 2) {
            if (overlay->id < (fsi_digest_table_end - fsi_digest_table_start) / 20)
                verified = FSi_CompareDigest(fsi_digest_table_start + overlay->id * 20,
                    overlay->address, size);
        }
        if (!verified) {
            MI_CpuFill8(overlay->address, 0, size);
            OS_Terminate();
            return;
        }
    }
    if (overlay->flags & 1) MIi_UncompressBackward(overlay->address + size);
    DC_FlushRange(overlay->address, overlay->ram_size);
    initializer = overlay->initializers;
    end = overlay->initializers_end;
    for (; initializer < end; initializer++) {
        if (*initializer) (*initializer)();
    }
}

void FS_EndOverlay(FsOverlayInfo *overlay) {
    for (;;) {
        FsDestructor *selected = 0;
        FsDestructor *tail;
        u32 ram_size = overlay->ram_size;
        u32 ram_start = (u32)overlay->address;
        u32 bss_size = overlay->bss_size;
        u32 ram_end = ram_start + ram_size;
        u32 bss_end = ram_end + FSi_GetOverlayBssLength(ram_size, bss_size);
        u32 state;
        FsDestructor *previous;
        FsDestructor *head;
        FsDestructor *current;
        tail = 0;
        state = OS_DisableInterrupts();
        head = __global_destructor_chain;
        previous = 0;
        current = head;
        while (current) {
            u32 callback = (u32)current->callback;
            FsDestructor *next = current->next;
            u32 argument = (u32)current->argument;
            if ((callback >= ram_start && callback < ram_end) ||
                (argument >= ram_end && argument < bss_end)) {
                if (tail) tail->next = current;
                else selected = current;
                if (head == current) {
                    __global_destructor_chain = next;
                    head = next;
                }
                current->next = 0;
                tail = current;
                if (previous) previous->next = next;
            } else {
                previous = current;
            }
            current = next;
        }
        OS_RestoreInterrupts(state);
        if (!selected) return;
        do {
            void (*callback)(void *) = selected->callback;
            FsDestructor *next = selected->next;
            if (callback) callback(selected->argument);
            selected = next;
        } while (selected);
    }
}

int FS_UnloadOverlayImage(FsOverlayInfo *overlay) {
    FS_EndOverlay(overlay);
    return 1;
}

int FS_LoadOverlay(int processor, u32 id) {
    FsOverlayInfo overlay;
    if (!FS_LoadOverlayInfo(&overlay, processor, id) || !FS_LoadOverlayImage(&overlay)) return 0;
    FS_StartOverlay(&overlay);
    return 1;
}

int FS_UnloadOverlay(int processor, u32 id) {
    FsOverlayInfo overlay;
    if (!FS_LoadOverlayInfo(&overlay, processor, id) || !FS_UnloadOverlayImage(&overlay)) return 0;
    return 1;
}
