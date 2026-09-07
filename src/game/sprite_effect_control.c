#include <game/sprite_effect.h>

extern const GameSpriteEffectFrame *data_02056650[];

void GameSpriteEffectPool_Release(GameSpriteEffectPool *pool, int mode, int id)
{
    GameSpriteEffect *effect = pool->active_list.first;
    switch (mode) {
    case 0:
        while (effect->next != (GameSpriteEffect *)-1) {
            GameSpriteEffect *next = effect->next;
            effect->previous->next = effect->next;
            effect->next->previous = effect->previous;
            pool->free_list.last->next = effect;
            effect->previous = pool->free_list.last;
            effect->next = (GameSpriteEffect *)&pool->free_list.last;
            pool->free_list.last = effect;
            effect = next;
        }
        break;
    case 1:
        while (effect->next != (GameSpriteEffect *)-1) {
            GameSpriteEffect *next = effect->next;
            if (effect->state.words.id == id) {
                effect->previous->next = effect->next;
                effect->next->previous = effect->previous;
                pool->free_list.last->next = effect;
                effect->previous = pool->free_list.last;
                effect->next = (GameSpriteEffect *)&pool->free_list.last;
                pool->free_list.last = effect;
            }
            effect = next;
        }
        break;
    }
}

void GameSpriteEffectPool_Resume(GameSpriteEffectPool *pool, int id)
{
    GameSpriteEffect *effect;
    for (effect = pool->active_list.first; effect->next != (GameSpriteEffect *)-1; effect = effect->next) {
        if (effect->state.words.id == id) {
            effect->state.bits.paused = 0;
            effect->state.bits.stop_at_end = 0;
        }
    }
}

void GameSpriteEffectPool_Pause(GameSpriteEffectPool *pool, int id, int mode)
{
    GameSpriteEffect *effect;
    for (effect = pool->active_list.first; effect->next != (GameSpriteEffect *)-1; effect = effect->next) {
        if (effect->state.words.id == id) {
            switch (mode) {
            case 0: effect->state.bits.paused = 1; break;
            case 1:
                effect->frame = data_02056650[effect->animation];
                effect->state.bits.paused = 1;
                break;
            case 2: effect->state.bits.stop_at_end = 1; break;
            }
        }
    }
}

void GameSpriteEffect_CopySize(GameSpriteEffectSize *destination, const GameSpriteEffectSize *source)
{
    *destination = *source;
}
