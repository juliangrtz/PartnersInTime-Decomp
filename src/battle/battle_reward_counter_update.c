/* Reward-label drawing (overlay 2, 0x0206C848-0x0206CB7C).
 * Draw the name one glyph at a time, then switch to two fixed-position digits.
 * The native second argument is unused; the third requests faster playback.
 */
#include "battle_reward_items_internal.h"
#include <game/text.h>
#include <game/battle_model_animation.h>
#include <game/battle_attack_loader.h>
#include <game/battle_context.h>
#include <game/battle_sound.h>
#include <game/battle_callback_model.h>
#define WORK data_ov002_020c06a4
#define TILE_BUFFER (*(void **)((u8 *)data_ov002_020c0660 + 508))
void func_ov002_0206c654(void *, int, int, int, int, int, int);
int BattleRewardCounter_Update(BattleRewardCounter *counter, int unused, int accelerate)
{
    GameText text;
    u8 *upload;
    *(u16 *)((u8 *)WORK + 1210 + 4 * counter->row) = 1;
    if (counter->flags.done)
        return 0;
    if (accelerate) {
        GameMatrixAnimationTrack *effect;
        counter->delay = 0;
        effect = (GameMatrixAnimationTrack *)counter->effect;
        if (effect && effect->commands)
            effect->speed = 0x4000;
    }
    if (counter->delay > 0) {
        --counter->delay;
        return 1;
    }
    if (!counter->flags.effect_started) {
        counter->effect = BattleModelAnimation_Start(
            759, 0, 76, (s16)(190 - 16 * (WORK->flags.bits.item_count + 1) + 16 * counter->row),
            (s16)(128 + 8 * counter->row), accelerate ? 1024 : 256);
        counter->flags.effect_started = 1;
        BattleSound_Play(242, 0, 0, 0);
        func_ov002_0206c654(TILE_BUFFER, 16, counter->x & ~7, counter->y & ~7, 128,
                            (counter->y & ~7) + 16, 0);
    }
    GameText_Init(&text, (const u32 *const *)(gBattleContext + 26868), TILE_BUFFER, counter->text,
                  (u8)counter->x, (u8)counter->y, 1, 1, 10, 6, 15, 0, 255, 0);
    counter->delay = 2;
    if (!counter->flags.drawing_count) {
        GameText_Next(&text, 0, 0);
        if (!*text.text) {
            counter->x = 100;
            counter->text = counter->digits;
            counter->flags.drawing_count = 1;
        } else {
            counter->x = text.cursor.bits.x;
            counter->text = (void *)text.text;
        }
    } else {
        text.style.bits.space_width = 8;
        GameText_Next(&text, 0, 0);
        text.cursor.bits.x = counter->x + 8;
        GameText_Next(&text, 0, 0);
        counter->flags.done = 1;
    }
    /* The label uses a callback renderer, whose owned buffer contains the
       upload record at +28. It is not a primary BattleModel allocation. */
    upload = (u8 *)(*(BattleCallbackModelView **)((u8 *)WORK + 208))->owned_buffer + 28;
    upload[60] &= ~1;
    upload[60] &= ~4;
    upload[60] |= 2;
    return !counter->flags.done;
}
