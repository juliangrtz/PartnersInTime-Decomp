#ifndef PIT_GAME_SCENE_WINDOW_H
#define PIT_GAME_SCENE_WINDOW_H
#include <nitro.h>
#ifdef __cplusplus
extern "C" {
#endif
void SceneWindow_Destroy(void);
void SceneText_LoadArchive(int entry);
int SceneText_IsLoadComplete(void);
u32 SceneText_GetEntrySize(const u32 *archive, int index);
void *SceneText_GetEntry(const u32 *archive, int index);
void SceneWindow_Close(int index);
int SceneWindow_IsOpen(int index);
int SceneWindow_Open(int text, int screen, int x, int y, int skin, int mode, int flag4, int style, int flag9,
                     int line_width, int width, int height, int flag18, int sound, int index, int value,
                     u32 unknown18);
#ifdef __cplusplus
}
#endif
#endif
