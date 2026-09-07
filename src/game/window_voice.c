#include <game/window.h>
#include <game/audio.h>
void GameWindow_StartTextSound(GameWindowManager *manager, GameWindow *window) {
    if (window->properties.shape.bits.sound) {
        GameAudio_StopEffect((s16)window->properties.shape.bits.sound);
        GameAudio_PlayEffectDelayed((s16)(window->properties.shape.bits.sound + 1), 0, -1);
        window->state.bits.animation = 3;
    }
}
