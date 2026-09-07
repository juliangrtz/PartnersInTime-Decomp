#include <game/sprite_image.h>

extern const GameSpriteEffectFrame *data_02056650[];
void GameSpriteEffectPool_Update(GameSpriteEffectPool *pool)
{
    const GameSpriteEffectFrame *frame;
    GameSpriteEffect *effect;
    GameSpriteEffect *drawing;
    for (effect = pool->active_list.first; effect->next != (GameSpriteEffect *)-1; effect = effect->next) {
        if (effect->update) effect->update(effect);
        if (!effect->state.bits.paused && !--effect->remaining) {
            ++effect->frame;
            switch (effect->frame->kind) {
            case 1:
                effect->frame = data_02056650[effect->animation];
                if (effect->state.bits.stop_at_end) {
                    effect->state.bits.stop_at_end = 0;
                    effect->state.bits.paused = 1;
                }
                break;
            case 2: break;
            }
            effect->remaining = effect->frame->duration;
        }
    }
    for (drawing = pool->active_list.first; drawing->next != (GameSpriteEffect *)-1; drawing = drawing->next) {
        GameSpriteEffectSize size;
        frame = drawing->frame;
        size.bytes.width = drawing->base_size.bytes.width;
        size.bytes.height = drawing->base_size.bytes.height;
        GameSpriteEffect_ScaleSize(frame->scale_x, frame->scale_y, &size);
        drawing->origin_x = ((size.bytes.width - drawing->base_size.bytes.width) >> 1) + 1;
        drawing->origin_y = ((size.bytes.height - drawing->base_size.bytes.height) >> 1) + 1;
        {
        GameSpriteEffectSize drawn = GameSpriteImage_Draw(drawing->x + frame->offset_x - drawing->origin_x,
            drawing->y + frame->offset_y - drawing->origin_y, frame->scale_x, frame->scale_y,
            drawing->image_index, drawing->palette, drawing->destination, drawing->image,
            drawing->state.bits.mode, 0);
        GameSpriteEffect_CopySize(&size, &drawn);
        }
        drawing->drawn_size.bytes.width = size.bytes.width;
        drawing->drawn_size.bytes.height = size.bytes.height;
    }
}

u32 GameSpriteEffectPool_Activate(GameSpriteEffectPool *pool, GameSpriteEffect *effect)
{
    s32 scale_x, scale_y;
    GameSpriteEffectSize size;
    effect->frame = data_02056650[effect->animation];
    effect->remaining = effect->frame->duration;
    effect->state.bits.stop_at_end = 0;
    scale_x = effect->frame->scale_x;
    scale_y = effect->frame->scale_y;
    size.bytes.width = GameSpriteImage_Width(effect->image, effect->image_index);
    size.bytes.height = (effect->image[0] & 15) * 4 - 1;
    effect->base_size.bytes.width = size.bytes.width;
    effect->base_size.bytes.height = size.bytes.height;
    if (scale_x && scale_y && (scale_x != 4096 || scale_y != 4096))
        GameSpriteEffect_ScaleSize(scale_x, scale_y, &size);
    effect->origin_x = ((size.bytes.width - effect->base_size.bytes.width) >> 1) + 1;
    effect->origin_y = ((size.bytes.height - effect->base_size.bytes.height) >> 1) + 1;
    effect->previous->next = effect->next;
    effect->next->previous = effect->previous;
    pool->active_list.last->next = effect;
    effect->previous = pool->active_list.last;
    effect->next = (GameSpriteEffect *)&pool->active_list.last;
    pool->active_list.last = effect;
    return size.raw + 0x10001;
}
