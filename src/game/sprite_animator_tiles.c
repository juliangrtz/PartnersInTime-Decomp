#include <game/sprite_animator.h>

extern const u16 data_02049ba8[], data_02049bb0[];
extern void MIi_CpuCopyFast(const void *source, void *destination, u32 size);

void GameSpriteAnimator_UploadTiles(GameSpriteAnimator *animator, int screen,
    GameSpriteAllocation *allocation, const void *tiles, u32 size,
    const GameSpriteAnimationFile *file, const GameSpriteAnimationObject *object)
{
    const u8 *source = tiles;
    u8 *destination;
    int count;
    u32 bytes;
    int bytes_per_tile;
    u8 alignment;
    int previous;
    u8 boundary;
    destination = GameSprite_ObjAddress(allocation);
    boundary = GameSprite_ObjBoundary(screen);
    if (boundary == data_02049ba8[file->flags0.tile_boundary]) {
        MIi_CpuCopyFast(source, destination, size);
        return;
    }
    count = file->object_count;
    previous = -1;
    bytes_per_tile = data_02049ba8[file->flags1.color256];
    if (count > 0) {
        alignment = boundary - 1;
        do {
            if ((int)object->tile > previous) {
                bytes = bytes_per_tile * data_02049bb0[(object->shape << 2) | object->size];
                MIi_CpuCopyFast(source, destination, bytes);
                source += bytes;
                destination += (bytes + alignment) & ~alignment;
            }
            --count;
            previous = object->tile;
            ++object;
        } while (count > 0);
    }
}
