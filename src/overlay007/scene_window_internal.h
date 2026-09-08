#ifndef PIT_SCENE_WINDOW_INTERNAL_H
#define PIT_SCENE_WINDOW_INTERNAL_H
#include <game/window.h>
#include <game/scene_window.h>
struct SceneWindowInterface {
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void unknown_0c();
    virtual int open(GameWindowProperties *, s16);
    virtual void close(s16);
};

extern u8 *data_ov007_020a6b90;
#define MANAGER (*(GameWindowManager **)data_ov007_020a6b90)
#define INTERFACE (*(SceneWindowInterface **)data_ov007_020a6b90)

#endif
