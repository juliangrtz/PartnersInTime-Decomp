#include "title_animation_internal.h"
extern "C" {
#include <game/heap.h>
#include <game/battle_archive.h>
#include <game/sprite_output.h>
extern const TitleAnimationArchive data_ov006_0207b038[5];
}

/* Uncompressed requests still carry the compressed queue state at +40. */
struct TitleArchiveReadRequest {
    ArchiveReadRequest read;
    u16 compressed_state, reserved;
};
typedef char TitleArchiveReadRequest_Size[sizeof(TitleArchiveReadRequest) == 44 ? 1 : -1];

extern "C" {
void TitleAnimation_ReleaseModelResources(TitleAnimationController *work)
{
    int i;
    TitleAnimationResource *resource = work->resources;
    for (i = 0; i < 16; ++i, ++resource) {
        if (resource->graphics) {
            GameHeap_Free(resource->graphics);
            resource->graphics = 0;
        }
        if (resource->unknown_08) {
            GameHeap_Free(resource->unknown_08);
            resource->unknown_08 = 0;
        }
        if (resource->unknown_0c) {
            GameHeap_Free(resource->unknown_0c);
            resource->unknown_0c = 0;
        }
        if (resource->unknown_10) {
            GameHeap_Free(resource->unknown_10);
            resource->unknown_10 = 0;
        }
        if (resource->owns_screen_group_offsets && resource->screen_group_offsets) {
            GameHeap_DeleteArray(resource->screen_group_offsets);
            resource->screen_group_offsets = 0;
        }
        if (resource->owns_screen_offsets && resource->screen_offsets) {
            GameHeap_DeleteArray(resource->screen_offsets);
            resource->screen_offsets = 0;
        }
        if (resource->owns_render_group_offsets && resource->render_group_offsets) {
            GameHeap_DeleteArray(resource->render_group_offsets);
            resource->render_group_offsets = 0;
        }
        if (resource->owns_render_offsets && resource->render_offsets) {
            GameHeap_DeleteArray(resource->render_offsets);
            resource->render_offsets = 0;
        }
    }
}

void *TitleAnimation_ReadArchiveEntry(TitleAnimationController *work, int archive, int entry,
                                     int compressed, int unused, int heap, u32 *size_out)
{
    ArchiveCompressedRequest compressed_request;
    TitleArchiveReadRequest raw_request;
    void *buffer;
    u32 size;
    if (compressed) {
        const char *descriptor = 0;
        if (work->current_descriptor != data_ov006_0207b038[archive].descriptor)
            descriptor = (const char *)data_ov006_0207b038[archive].descriptor;
        compressed_request.read.path = 0;
        compressed_request.read.entry = entry;
        size = ArchiveIO_PrepareCompressedRead(&work->archive->base, &compressed_request,
                                               work->offsets[archive], descriptor);
        buffer = GameHeap_Allocate(heap, size, 0, 1);
        compressed_request.destination = (u8 *)buffer;
        ArchiveIO_QueueCompressedRead(&work->archive->base, &compressed_request, buffer);
        ArchiveIO_WaitCompressedRead(&work->archive->base, &compressed_request.read);
    } else {
        size = BattleArchive_GetEntrySize(work->archive, (const u8 *)work->offsets[archive], (u16)entry);
        buffer = GameHeap_Allocate(heap, size, 0, 1);
        raw_request.read.path = 0;
        raw_request.read.entry = entry;
        raw_request.read.limit = size;
        raw_request.read.destination = (u8 *)buffer;
        const void *descriptor = 0;
        if (work->current_descriptor != data_ov006_0207b038[archive].descriptor)
            descriptor = data_ov006_0207b038[archive].descriptor;
        BattleArchive_ReadAsync(work->archive, &raw_request, 0,
                                (const u8 *)work->offsets[archive], descriptor, 0);
        ArchiveIO_WaitCompressedRead(&work->archive->base, &raw_request.read);
    }
    if (size_out)
        *size_out = size;
    return buffer;
}
}
