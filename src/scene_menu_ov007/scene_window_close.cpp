/*
 * Scene window close (overlay 7, 0x0208953C-0x020895B8).
 *
 * Closes the scene's message window.
 */

#include "scene_window_internal.h"

extern "C" {

void SceneWindow_Close(int index)
{
    if (index >= 0)
        INTERFACE->close((s16)index);
    else
        for (int i = 0; i < 4; ++i)
            INTERFACE->close((s16)i);
}
}
