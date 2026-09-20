/*
 * Battle number OAM (overlay 2, 0x020684D8-0x02068770).
 * Draw up to three digits right-to-left, eleven pixels apart. A nonzero
 * leading setting draws one additional zero: 1 follows the emitted digits;
 * other values place it relative to the original X coordinate.
 * The caller supplies an unsigned 16-bit value, but the native divisions here
 * operate on signed words and truncate each remaining quotient to 16 bits.
 */
#include "battle_oam_internal.h"
#include <game/battle_attack_loader.h>
typedef struct NumberSpriteWork {
    u8 unknown_000[432];
    u16 offsets[34];
    GameOamEntry *objects;
} NumberSpriteWork;
#define WORK ((NumberSpriteWork *)data_ov002_020c0660)
void BattleOam_DrawNumber(GameOamEntry *output, u8 *object_count, u8 *affine_count, int x, int y,
                          int value, int leading, struct BattleOamTransform *transform)
{
    int offset = WORK->offsets[value % 10];
    int position;
    int original_x = x;
    func_ov002_0206acbc(output, object_count, affine_count, WORK->objects + offset,
                        (u8)(WORK->offsets[value % 10 + 1] - offset), (s16)x, (s16)y, transform, 1);
    x -= 11;
    value = (u16)(value / 10);
    position = 1;
    do {
        if (!value)
            break;
        offset = WORK->offsets[value % 10];
        func_ov002_0206acbc(output, object_count, affine_count, WORK->objects + offset,
                            (u8)(WORK->offsets[value % 10 + 1] - offset), (s16)x, (s16)y, transform,
                            1);
        ++position;
        x -= 11;
        value = (u16)(value / 10);
    } while (position < 3);
    if (leading) {
        if (leading == 1)
            func_ov002_0206acbc(
                output, object_count, affine_count, WORK->objects + WORK->offsets[0],
                (u8)(WORK->offsets[1] - WORK->offsets[0]), (s16)x, (s16)y, transform, 1);
        else
            func_ov002_0206acbc(output, object_count, affine_count,
                                WORK->objects + WORK->offsets[0],
                                (u8)(WORK->offsets[1] - WORK->offsets[0]),
                                (s16)(original_x - 11 * leading), (s16)y, transform, 1);
    }
}
