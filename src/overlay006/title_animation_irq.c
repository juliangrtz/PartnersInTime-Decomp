#include <game/title_animation.h>
#include <game/heap.h>
#include <game/input.h>
#include <game/battle_scene.h>
#include <game/texture_allocation.h>
#include <game/sprite_output.h>

typedef struct TitleAnimationIrq {
    GameIrqTask base;
    u8 unknown_18[16];
    u32 idle_frames;
    u32 frame_requested : 1;
    u32 unknown_01_31 : 31;
    u32 unknown_30;
} TitleAnimationIrq;
typedef char TitleAnimationIrq_Size[sizeof(TitleAnimationIrq) == 52 ? 1 : -1];

extern GameTaskVTable data_ov006_0207b1ac;
extern const u32 data_ov006_0207b134[], data_ov006_0207b1e8[];
void func_0202cbd4(void *, int, unsigned);
u32 func_02035818(void);
u32 func_02035804(void);
void func_02035b0c(u32);
void func_02035c00(u32);
void func_ov006_0206c484(void);
void func_020090b8(int);

GameIrqTask *TitleAnimationIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    TitleAnimationIrq *work = (TitleAnimationIrq *)task;
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov006_0207b1ac;
    func_0202cbd4(&work->idle_frames, 0, (u8 *)&work->unknown_30 - (u8 *)&work->idle_frames);
    return task;
}

GameIrqTask *TitleAnimationIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207b1ac;
    GameIrqTask_DestroyBase(task);
    return task;
}

GameIrqTask *TitleAnimationIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207b1ac;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

void TitleAnimationIrq_UploadFrame(GameIrqTask *task)
{
    TitleAnimationIrq *work = (TitleAnimationIrq *)task;
    u32 texture_banks, palette_banks;
    if (!work->frame_requested) {
        ++work->idle_frames;
        return;
    }
    texture_banks = func_02035818();
    palette_banks = func_02035804();
    BattleRenderModels_UpdateTextures(data_ov006_0207b134, 3);
    GameTexturePalette_Upload(data_ov006_0207b1e8);
    func_02035b0c(palette_banks);
    func_02035c00(texture_banks);
    func_ov006_0206c484();
    func_020090b8(0);
    func_020090b8(1);
    GameSpritePalette_UploadScreen(0);
    GameSpritePalette_UploadScreen(1);
    GameDisplay_CopyOam(3);
    GameDisplay_ClearOam();
    work->idle_frames = 0;
    work->frame_requested = 0;
}
