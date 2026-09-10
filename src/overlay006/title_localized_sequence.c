#include <game/title_localized_sequence.h>
#include <game/heap.h>
extern u8 *data_ov006_0207c4e4;
void *func_ov006_0206b77c(void *, int, int, int, int, int, u32 *);
int func_02035818(void);
void func_02035c00(int);
void func_ov006_02070ed8(TitleLocalizedSequence *);

static inline void DeleteSpriteAnimation(GameSpriteAnimation *animation)
{
    if (animation) {
        GameSpriteAnimation_Finalize(animation);
        GameHeap_Delete(animation);
    }
}

void TitleLocalizedSequence_Init(TitleLocalizedSequence *work)
{
    GameSpriteAnimation *animation = GameHeap_New(sizeof(GameSpriteAnimation), 1, 0, 0);
    int bank;
    if (animation)
        animation = GameSpriteAnimation_Initialize(animation, work->track_storage, 4, 1024);
    work->animation = animation;
    TitleTexture_Load(&work->textures[0], 17, 20, 1);
    TitleTexture_Load(&work->textures[2], 19, 20, 1);
    TitleTexture_Load(&work->textures[3], 13, 14, 1);
    TitleTexture_Load(&work->textures[4], 30, 20, 1);
    TitleTexture_Load(&work->textures[1], data_ov006_0207c4e4[1048] + 24, 20, 1);
    bank = func_02035818();
    TitleTexture_Upload(&work->textures[0]);
    TitleTexture_Upload(&work->textures[2]);
    TitleTexture_Upload(&work->textures[3]);
    TitleTexture_Upload(&work->textures[4]);
    TitleTexture_Upload(&work->textures[1]);
    func_02035c00(bank);
    work->commands[0] = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 15, 0, 0, 1, 0);
    work->commands[1] = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 16, 0, 0, 1, 0);
    work->header.x = 0x80000;
    work->header.y = 0x38000;
    work->header.draw = (void (*)(void *))func_ov006_02070ed8;
}

void TitleLocalizedSequence_Release(TitleLocalizedSequence *work)
{
    GameSpriteAnimation *animation = work->animation;
    if (animation) {
        DeleteSpriteAnimation(animation);
        work->animation = 0;
    }
    TitleTexture_Release(&work->textures[0]);
    TitleTexture_Release(&work->textures[2]);
    TitleTexture_Release(&work->textures[3]);
    TitleTexture_Release(&work->textures[4]);
    TitleTexture_Release(&work->textures[1]);
    if (work->commands[0]) {
        GameHeap_Free((void *)work->commands[0]);
        work->commands[0] = 0;
    }
    if (work->commands[1]) {
        GameHeap_Free((void *)work->commands[1]);
        work->commands[1] = 0;
    }
}

void TitleLocalizedSequence_Start(TitleLocalizedSequence *work)
{
    work->header.state = 1;
}

void TitleLocalizedSequence_Finish(TitleLocalizedSequence *work)
{
    work->visible_parts.mask = 0xff;
    work->header.x = 0x80000;
    work->header.y = 0x38000;
    if (work->tracks[0])
        work->tracks[0]->commands = 0;
    if (work->tracks[1])
        work->tracks[1]->commands = 0;
    work->header.state = 0;
}
