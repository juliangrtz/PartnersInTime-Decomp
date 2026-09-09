#ifndef PIT_GAME_BATTLE_CALLBACK_MODEL_H
#define PIT_GAME_BATTLE_CALLBACK_MODEL_H
#include <game/battle_scene.h>

/* Controller subclass with an owned buffer and three overridable callbacks. */
typedef struct BattleCallbackModelView {
    u8 controller[304];
    void *owned_buffer;
    void (*prepare)(BattleModel *);
    void (*draw)(BattleModel *, void *, u8 *, u8 *);
    void (*update)(BattleModel *);
} BattleCallbackModelView;
typedef char BattleCallbackModelViewSizeCheck[sizeof(BattleCallbackModelView) == 320 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void BattleCallbackModel_Update(BattleModel *model);
void BattleCallbackModel_Draw(BattleModel *model, void *buffer, u8 *object_count, u8 *affine_count);
void BattleCallbackModel_Prepare(BattleModel *model);
void BattleCallbackModel_IgnoreAnimation(BattleModel *model, u8, int, int);
void BattleCallbackModel_ReleaseNoOp(BattleModel *model);
void BattleCallbackModel_BindPalette(BattleModel *model, const void *descriptor, void *, s16);
BattleModel *BattleCallbackModel_Delete(BattleModel *model);
BattleModel *BattleCallbackModel_Destroy(BattleModel *model);
BattleModel *BattleCallbackModel_Init(BattleModel *model);
#ifdef __cplusplus
}
#endif
#endif
