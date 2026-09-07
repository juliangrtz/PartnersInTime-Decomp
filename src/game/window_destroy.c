#include <game/window.h>
extern void *data_02056534[];
extern void GameWindow_ReleaseAll(GameWindowManager *);
/* The original delete path retains the inner null check after inlining. */
static inline void DeleteAnimator(GameSpriteAnimator *animator) {
    if (animator) { GameSpriteAnimator_Destroy(animator); GameHeap_Delete(animator); }
}
static inline void Destroy(GameWindowManager *manager) {
    GameSpriteAnimator *animator;
    GameSpriteEffectPool *effects;
    manager->vtable = data_02056534;
    GameIrqTask_DeleteSafe(manager->irq_task);
    animator = manager->animator;
    GameSpriteAllocation_Unlink(&animator->allocation[0]);
    animator->state.bits.main_allocated = 0;
    animator = manager->animator;
    GameSpriteAllocation_Unlink(&animator->allocation[1]);
    animator->state.bits.sub_allocated = 0;
    GameWindow_ReleaseAll(manager);
    animator = manager->animator;
    if (animator) DeleteAnimator(animator);
    effects = manager->effects;
    if (effects) { GameSpriteEffectPool_Destroy(effects); GameHeap_Delete(effects); }
    if (manager->owned_buffer) GameHeap_DeleteArray(manager->owned_buffer);
    GameTask_DestroyBase((GameTask *)manager);
}
GameWindowManager *GameWindow_DestroyComplete(GameWindowManager *manager) { Destroy(manager); return manager; }
GameWindowManager *GameWindow_Delete(GameWindowManager *manager) { Destroy(manager); GameHeap_Delete(manager); return manager; }
GameWindowManager *GameWindow_DestroyBase(GameWindowManager *manager) { Destroy(manager); return manager; }
