extern "C" {
#include <game/menu_item_effect.h>
#include <game/save_state_transfer.h>
MenuItemEffectTask *func_ov005_0206659c(void (*)(MenuItemEffectTask *), int, int);
void func_ov005_0206650c(MenuItemEffectTask *);
MenuItemEffectSprite *Overlay5ResourceB_Attach(MenuItemEffectTask *, MenuItemEffectSprite *, int);
MenuItemEffectSprite *Overlay5ResourceB_Get(MenuItemEffectTask *);
void func_ov005_020695ec(MenuItemEffectSprite *, int);
void func_ov005_02069330(MenuItemEffectSprite *, int);
void func_ov005_020692c0(MenuItemEffectSprite *, u16, u16, int);
void func_ov005_02069084(MenuItemEffectSprite *, int);
void func_02036988(vu16 *, int, int, int, int);
void func_ov005_02069bcc(int, int, int, int);
int ItemEffect_CalculateValue(u16, int, int, int);
void func_ov007_02077e84(MenuItemEffectTask *, int, int, int, int, int, int, int, int);
}

extern "C" void MenuItemGlow_Update(MenuItemEffectTask *task)
{
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Get(task);
    switch (task->state) {
    case MENU_GLOW_DELAY:
        if (task->counter)
            --task->counter;
        else
            ++task->state;
        break;
    case MENU_GLOW_FADE:
        task->glow.alpha -= 4096 / task->glow.duration;
        if (task->glow.alpha <= 0) {
            if (task->parent->owner.children)
                --task->parent->owner.children;
            func_ov005_0206650c(task);
            return;
        }
        break;
    }
    int alpha = 16 * task->glow.alpha / 4096;
    if (alpha > 16)
        alpha = 16;
    func_02036988((vu16 *)0x4001050, 0, 31, alpha, 16);
    task->glow.scale += task->glow.velocity;
    task->glow.velocity += task->glow.acceleration;
    func_ov005_020692c0(sprite, task->glow.scale / 16, task->glow.scale / 16, 0);
    func_ov005_02069084(sprite, 41);
}
extern "C" void MenuItemGlow_Create(MenuItemEffectTask *parent, int x, int y, int duration)
{
    MenuItemEffectTask *task = func_ov005_0206659c(MenuItemGlow_Update, 8, 1);
    task->counter = 2;
    task->glow.alpha = 4096;
    task->glow.duration = duration;
    task->glow.scale = 0;
    task->glow.velocity = 9830 / duration;
    task->glow.acceleration = -task->glow.velocity / duration;
    task->parent = parent;
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Attach(task, 0, 1);
    func_ov005_020695ec(sprite, 1);
    sprite->attributes_1 = (sprite->attributes_1 & ~0x3FF) | 0x61;
    sprite->attributes_1 = (sprite->attributes_1 & ~0xF000) | 0x3000;
    sprite->attributes_0 &= ~0x2000;
    sprite->attributes_0 = (sprite->attributes_0 & ~0x300) | 0x300;
    sprite->attributes_1 = (sprite->attributes_1 & ~0xC00) | 0x400;
    sprite->attributes_0 = (sprite->attributes_0 & ~0xC00) | 0x400;
    func_ov005_02069330(sprite, 2);
    sprite->x = x;
    sprite->y = y;
    sprite->center_x = 32;
    sprite->center_y = 32;
    ++parent->owner.children;
}
extern "C" void MenuItemEffect_Start(MenuItemEffectTask *parent, int x, int y)
{
    func_02036988((vu16 *)0x4000050, 0, 31, 0, 16);
    parent->owner.children = 0;
    func_ov007_02077e84(parent, x, y, 20, 32, 2457, 8738, 8192, 0);
    func_ov007_02077e84(parent, x, y, 25, 32, 3276, 4369, 4096, 1);
    func_ov007_02077e84(parent, x, y, 30, 32, 4096, 0, 0, 2);
    MenuItemGlow_Create(parent, x, y, 16);
    func_ov005_02069bcc(334, 0, 0, 128);
}

extern "C" void MenuItemHeal_Update(MenuItemEffectTask *task)
{
    switch (task->state) {
    case MENU_HEAL_START:
        ++task->state;
        break;
    case MENU_HEAL_APPLY: {
        SavePartyMember *party = &((SaveLiveTransferView *)gSaveData)->party[task->heal.member];
        party->current_hp = ItemEffect_CalculateValue(task->heal.item, party->current_hp, party->max_hp, 0);
        MenuItemEffect_Start(task, task->heal.x, task->heal.y);
        ++task->state;
        break;
    }
    case MENU_HEAL_WAIT:
        if (!task->owner.children) {
            if (task->parent->owner.children)
                --task->parent->owner.children;
            func_ov005_0206650c(task);
        }
        break;
    }
}
