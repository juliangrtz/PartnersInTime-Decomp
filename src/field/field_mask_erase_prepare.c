/* Load the four mask/cursor images asynchronously and preserve the main OBJ
 * graphics before the touchscreen mask-erase effect uses their VRAM bank. */
#include <game/field_system.h>
#include <game/heap.h>
#include <game/battle_archive.h>
#include <game/sprite_output.h>
extern const u32 data_ov000_020bf630[4];
void func_0202cbd4(void *, int, u32);
void FieldSystem_PrepareMaskErase(FieldSystem *system)
{
    FieldPendingTransfer *state = GameHeap_New(sizeof(FieldPendingTransfer), 0, 0, 1);
    int image;
    func_0202cbd4(state, 0, sizeof(FieldPendingTransfer));
    for (image = 0; image < 4; ++image) {
        const char *descriptor;
        state->requests[image].read.entry = data_ov000_020bf630[image];
        state->requests[image].read.limit = BattleArchive_GetEntrySize(
            system->archive, system->archive->descriptors[6], state->requests[image].read.entry);
        state->requests[image].read.destination = GameHeap_NewArray(
            state->requests[image].read.limit, 0, 0, 1);
        state->images[image] = state->requests[image].read.destination;
        state->image_sizes[image] = state->requests[image].read.limit;
        descriptor = FieldArchive_GetDescriptor(system->archive, 6);
        BattleArchive_ReadAsync(system->archive, &state->requests[image], 0,
            system->archive->descriptors[6], descriptor, 0x8000);
    }
    GameSprite_CopyBytes((const void *)0x06400000, state->saved_object_tiles, 131072);
    system->pending_transfer = state;
}
