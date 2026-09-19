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
