#include <game/title_sprite_sequence.h>

extern const u8 data_ov006_0207b2c4[];
void func_ov006_0207055c(TitleSequenceActor *);
void func_ov006_02070c70(void *);
void func_ov006_020705dc(TitleSequenceActor *);
void func_ov006_02070660(TitleSequenceActor *);
void func_ov006_020718fc(TitleTextureResource *);
int func_02035818(void);
void func_02035c00(int);
void func_ov006_02070588(TitleSequenceActor *, int, int, int);
int func_ov006_02070548(TitleSequenceActor *);
void func_ov006_0207047c(TitleSequenceSprite *, int, int, int, int, int, int, int, int, int);
void func_ov006_0207044c(TitleSequenceSprite *, int, int, int, int, int);
int func_ov006_02075120(TitleSequenceActor *);
int func_ov006_020703e0(TitleSequenceSprite *, int);
void func_ov006_02070c58(void *);

void func_ov006_0206f3f0(TitleFadeSprite *);

void TitleFadeSprite_Init(TitleFadeSprite *work, TitleTextureResource *texture)
{
    work->texture = texture;
    work->layout = data_ov006_0207b2c4;
    work->header.update = (void (*)(void *))TitleFadeSprite_Update;
    work->header.draw = (void (*)(void *))func_ov006_0206f3f0;
    work->header.x = 0x80000;
    work->header.y = 278528;
    work->header.state = 0;
}

void TitleFadeSprite_StartFadeIn(TitleFadeSprite *work)
{
    work->header.elapsed = 0;
    work->header.duration = 32;
    work->header.state = 1;
}

void TitleFadeSprite_Show(TitleFadeSprite *work)
{
    work->alpha = 31;
    work->header.state = 2;
}

void TitleSpriteSequence_Update(TitleSpriteSequence *work)
{
    ++work->elapsed;
    if (work->header.state > 1)
        ++work->active_elapsed;
    work->actor.header.update(&work->actor);
    work->sprites[0].header.update(&work->sprites[0]);
    work->sprites[1].header.update(&work->sprites[1]);
    work->rotating.sprite.header.update(&work->rotating);
    work->fade.header.update(&work->fade);
    switch (work->header.state) {
    case 0:
        break;
    case 1:
        if (work->elapsed > 435) {
            func_ov006_02070588(&work->actor, 0, 0, 4096);
            work->header.state = 2;
        }
        break;
    case 2:
        if (func_ov006_02070548(&work->actor)) {
            func_ov006_0207047c(&work->sprites[0], 16, 40, 20, 128, 84, 128, 76, 64, 8);
            func_ov006_0207047c(&work->sprites[1], 40, 40, 16, 128, 84, 220, 72, 80, 12);
            work->header.state = 3;
        }
        break;
    case 3:
        if (TitleSequenceSprite_IsSquashing(&work->sprites[1])) {
            func_ov006_0207044c(&work->sprites[0], 128, 76, 114, 76, 16);
            work->header.state = 4;
        }
        break;
    case 4:
        if (TitleSequenceSprite_IsIdle(&work->sprites[0]) && TitleSequenceSprite_IsIdle(&work->sprites[1]) &&
            func_ov006_02075120(&work->actor)) {
            TitleRotatingSprite_StartArc(&work->rotating, 24, 0, 219, -64, 219, 40, 0, -104, 1);
            work->header.state = 5;
        }
        break;
    case 5:
        if (TitleRotatingSprite_IsIdle(&work->rotating)) {
            int duration = func_ov006_020703e0(&work->sprites[1], 20);
            TitleRotatingSprite_StartVerticalMove(&work->rotating, duration);
            work->header.state = 7;
        }
        break;
    case 6:
        break;
    case 7:
        if (TitleSequenceSprite_IsIdle(&work->sprites[1])) {
            func_ov006_0207044c(&work->sprites[0], 114, 76, 100, 76, 12);
            func_ov006_0207044c(&work->sprites[1], 220, 72, 206, 72, 12);
            work->header.state = 8;
        }
        break;
    case 8:
        if (TitleSequenceSprite_IsIdle(&work->sprites[0]) && TitleSequenceSprite_IsIdle(&work->sprites[1]) &&
            TitleRotatingSprite_IsIdle(&work->rotating)) {
            TitleFadeSprite_StartFadeIn(&work->fade);
            work->header.state = 9;
        }
        break;
    case 9:
        if (work->fade.header.state == 2) {
            func_ov006_02070c58(&work->sprites[0]);
            func_ov006_02070c58(&work->sprites[1]);
            func_ov006_02070c58(&work->rotating);
            work->header.state = 0;
        }
        break;
    }
}

void TitleSpriteSequence_Draw(TitleSpriteSequence *work)
{
    work->actor.header.draw(&work->actor);
    work->sprites[0].header.draw(&work->sprites[0]);
    work->sprites[1].header.draw(&work->sprites[1]);
    work->rotating.sprite.header.draw(&work->rotating);
    work->fade.header.draw(&work->fade);
}

void TitleSpriteSequence_Init(TitleSpriteSequence *work)
{
    int bank;
    TitleTexture_Load(&work->textures[0], 21, 23, 1);
    TitleTexture_Load(&work->textures[1], 22, 23, 1);
    bank = func_02035818();
    func_ov006_020718fc(&work->textures[0]);
    func_ov006_020718fc(&work->textures[1]);
    func_02035c00(bank);
    func_ov006_02070660(&work->actor);
    TitleSequenceSprite_Init(&work->sprites[0], &work->textures[0], 0, 1584, 1584);
    TitleSequenceSprite_Init(&work->sprites[1], &work->textures[0], 1, 1600, 1568);
    TitleRotatingSprite_Init(&work->rotating, &work->textures[1]);
    TitleFadeSprite_Init(&work->fade, &work->textures[0]);
    work->header.update = (void (*)(void *))TitleSpriteSequence_Update;
    work->header.draw = (void (*)(void *))TitleSpriteSequence_Draw;
    work->header.state = 0;
}

void TitleSpriteSequence_Release(TitleSpriteSequence *work)
{
    TitleTexture_Release(&work->textures[0]);
    TitleTexture_Release(&work->textures[1]);
    func_ov006_020705dc(&work->actor);
}

void TitleSpriteSequence_Start(TitleSpriteSequence *work)
{
    work->header.state = 1;
}

void TitleSpriteSequence_Finish(TitleSpriteSequence *work)
{
    func_ov006_0207055c(&work->actor);
    func_ov006_02070c70(&work->sprites[0]);
    func_ov006_02070c70(&work->sprites[1]);
    func_ov006_02070c70(&work->rotating);
    TitleFadeSprite_Show(&work->fade);
    work->header.state = 0;
}
