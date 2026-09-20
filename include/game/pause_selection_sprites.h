#ifndef PIT_GAME_PAUSE_SELECTION_SPRITES_H
#define PIT_GAME_PAUSE_SELECTION_SPRITES_H
#include <game/pause_scene.h>
#include <game/pause_navigation.h>

/* Category cursor origin in pixels; the selected category adds a row offset. */
typedef struct PauseSelectionSpriteTask {
    u8 unknown_00[40];
    int x, y;
    u8 unknown_30[24];
} PauseSelectionSpriteTask;
typedef char PauseSelectionSpriteTaskSize[sizeof(PauseSelectionSpriteTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void PauseList_CreateSelectionSprites(PausePageTask *parent, int suppress_heading);
void PauseEquipment_CreateCategoryCursor(void);
void PauseEquipment_UpdateCategoryCursor(PauseSelectionSpriteTask *task);
void PauseList_UpdateSelectionMarker(PauseMenuElement *task);
void PauseList_UpdateSelectionCursor(PauseMenuElement *task);
#ifdef __cplusplus
}
#endif
#endif
