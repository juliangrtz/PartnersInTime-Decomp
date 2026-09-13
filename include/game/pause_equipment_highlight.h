#ifndef PIT_GAME_PAUSE_EQUIPMENT_HIGHLIGHT_H
#define PIT_GAME_PAUSE_EQUIPMENT_HIGHLIGHT_H

typedef struct PauseEquipmentHighlightTask PauseEquipmentHighlightTask;
#ifdef __cplusplus
extern "C" {
#endif
void PauseEquipmentHighlight_Stop(void);
void PauseEquipmentHighlight_Start(void);
void PauseEquipmentHighlight_Update(PauseEquipmentHighlightTask *task);
#ifdef __cplusplus
}
#endif
#endif
