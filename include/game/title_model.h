#ifndef PIT_GAME_TITLE_MODEL_H
#define PIT_GAME_TITLE_MODEL_H
#include <game/battle_scene.h>

#ifdef __cplusplus
extern "C" {
#endif
/* Title elements share the coordinate/callback prefix and two model slots.
 * The concrete orbit, moving-sprite and menu records have different tails. */
void TitleModel_Draw(void *element);
void TitleModel_DrawSub(BattleModel *model);
void TitleModel_DrawMain(BattleModel *model);
void TitleModel_Release(void *element);
BattleModel *TitleModelController_Delete(BattleModel *model);
void TitleModel_Load(void *element, u32 resource, u8 screens, int parameter, int mode);
#ifdef __cplusplus
}
#endif
#endif
