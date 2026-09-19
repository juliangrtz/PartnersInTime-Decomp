extern "C" {
#include <game/sprite_animator.h>
}

class GameSpriteAnimatorInterface {
public:
    virtual void draw(GameSpriteAnimation *entry);
};

extern "C" {

void GameSpriteAnimator_Update(GameSpriteAnimator *animator)
{
    GameSpriteAnimation *entry;
    animator->state.bytes.count[0] = 0;
    animator->state.bytes.count[1] = 0;
    for (entry = animator->list.first; entry->next != (GameSpriteAnimation *)-1; entry = entry->next) {
        if (entry->flags.bits.animated) {
            if (!entry->flags.bits.paused && --entry->data.animation.remaining < 0) {
                if (++entry->data.animation.frame >= (u32)entry->data.animation.end)
                    entry->data.animation.frame = entry->data.animation.first;
                entry->data.animation.remaining = animator->frames[entry->data.animation.frame].duration;
            }
            ((GameSpriteAnimatorInterface *)animator)->draw(entry);
        } else GameSpriteAnimator_DrawDirect(animator, entry);
    }
}

}
