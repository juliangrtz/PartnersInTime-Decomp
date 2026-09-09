#ifndef PIT_GAME_MENU_ITEM_EFFECT_H
#define PIT_GAME_MENU_ITEM_EFFECT_H
#include <game/overlay005_resource.h>

enum MenuItemHealState { MENU_HEAL_START, MENU_HEAL_APPLY, MENU_HEAL_WAIT };
enum MenuItemGlowState { MENU_GLOW_DELAY, MENU_GLOW_FADE };
enum MenuItemStarState { MENU_STAR_GROW, MENU_STAR_HOLD, MENU_STAR_SHRINK };

/* A shared 72-byte menu task with effect-specific payloads. */
typedef struct MenuItemEffectTask {
    u8 unknown_00[16];
    struct MenuItemEffectTask *parent;
    u8 unknown_14[12];
    int state, counter;
    union {
        struct {
            int alpha, duration, scale, velocity, acceleration;
            u8 unknown_3c[12];
        } glow;
        struct {
            int velocity, acceleration, angle, rotation, scale;
            u16 delay, duration, frame, tick;
            u8 unknown_44[4];
        } star;
        struct {
            int member, item, x, y;
            u8 unknown_38[16];
        } heal;
        struct {
            u8 unknown_28[20];
            int children;
            u8 unknown_40[8];
        } owner;
    };
} MenuItemEffectTask;
typedef Overlay5Sprite MenuItemEffectSprite;
typedef struct MenuItemStarFrames {
    u8 steps[5][2];
} MenuItemStarFrames;
typedef char MenuItemEffectTask_SizeCheck[sizeof(MenuItemEffectTask) == 72 ? 1 : -1];
typedef char MenuItemEffectSprite_SizeCheck[sizeof(MenuItemEffectSprite) == 64 ? 1 : -1];
typedef char MenuItemStarFrames_SizeCheck[sizeof(MenuItemStarFrames) == 10 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void MenuItemHeal_Update(MenuItemEffectTask *task);
void MenuItemEffect_Start(MenuItemEffectTask *parent, int x, int y);
void MenuItemGlow_Create(MenuItemEffectTask *parent, int x, int y, int duration);
void MenuItemGlow_Update(MenuItemEffectTask *task);
void MenuItemStar_Update(MenuItemEffectTask *task);
#ifdef __cplusplus
}
#endif
#endif
