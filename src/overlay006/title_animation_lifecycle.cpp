#include "title_animation_internal.h"
#include <game/title_startup.h>
extern "C" {
#include <game/heap.h>
#include <game/archive_io.h>
#include <game/battle_archive.h>
#include <game/audio.h>
#include <game/input.h>
#include <game/save_data.h>
#include <game/rumble.h>
#include <nns/snd.h>
}


extern "C" {
extern GameInput data_0206032c;
extern GameTaskVTable data_ov006_0207b198, data_ov006_0207b38c;
extern TitleAnimationController *data_ov006_0207c4e4;
extern const TitleAnimationArchive data_ov006_0207b038[5];
extern const GameRumblePattern data_ov006_0207af7c;
void func_0202cbd4(void *, int, unsigned);
void func_ov006_0206d2b4(void);
}

static inline void DeleteTask(void *object)
{
    if (object)
        ((GameTaskDispatch *)object)->delete_task();
}

extern "C" {
TitleAnimationController *TitleAnimation_Init(TitleAnimationController *work, u32 priority, u32 unused, void *argument)
{
    GameTask_Init(&work->base, priority, unused, argument);
    work->base.vtable = &data_ov006_0207b198;
    data_ov006_0207c4e4 = work;
    func_0202cbd4(&work->irq, 0, (u8 *)(work + 1) - (u8 *)&work->irq);
    if (gSaveData)
        work->language = ((SaveLocalizedResourceState *)gSaveData)->language;
    else
        work->language = 0;
    GameIrqTask *irq = (GameIrqTask *)GameHeap_New(52, 1, 0, 1);
    if (irq)
        irq = TitleAnimationIrq_Init(irq, 8, 0, work);
    work->irq = irq;
    ArchiveCompressedIO *archive = (ArchiveCompressedIO *)GameHeap_New(1088, 1, 0, 1);
    if (archive)
        archive = ArchiveCompressedIO_InitBase(archive, (u8)(priority + 10), 0, work, -32897);
    work->archive = archive;
    GameTask *frame_task = (GameTask *)GameHeap_New(40, 1, 0, 1);
    if (frame_task) {
        GameTask_Init(frame_task, (u8)(priority + 20), 0, work);
        frame_task->vtable = &data_ov006_0207b38c;
    }
    work->frame_task = frame_task;
    TitleAnimation_InitDisplay(work);
    TitleAnimation_LoadArchiveTables(work);
    TitleAnimation_LoadLocalizedResources(work);
    work->previous_first_delay = data_0206032c.first_delay;
    work->previous_repeat_delay = data_0206032c.repeat_delay;
    data_0206032c.first_delay = 20;
    data_0206032c.repeat_delay = 8;
    GameRumble_SetPatterns(&data_ov006_0207af7c, 1);
    NNS_SndPlayerSetAllocatableChannel(0, 65525);
    func_ov006_0206d2b4();
    GameIrqTask_Enable(work->irq);
    return work;
}

TitleAnimationController *TitleAnimation_Destroy(TitleAnimationController *work)
{
    work->base.vtable = &data_ov006_0207b198;
    GameIrqTask_Disable(work->irq);
    GameAudio_StopMusic(0);
    while (GameAudio_GetMusicState() == 1) {}
    NNS_SndPlayerSetAllocatableChannel(0, 24565);
    if (work->irq) {
        GameIrqTask_DeleteSafe(work->irq);
        work->irq = 0;
    }
    if (work->archive) {
        DeleteTask(work->archive);
        work->archive = 0;
    }
    if (work->frame_task) {
        DeleteTask(work->frame_task);
        work->frame_task = 0;
    }
    TitleAnimation_ReleaseArchiveTables(work);
    TitleAnimation_ReleaseModelResources(work);
    TitleAnimation_ReleaseRenderState(work);
    TitleAnimation_ReleaseLocalizedResources(work);
    TitleAnimation_ReleaseSequence();
    data_0206032c.first_delay = work->previous_first_delay;
    data_0206032c.repeat_delay = work->previous_repeat_delay;
    GameTask_DestroyBase(&work->base);
    return work;
}

TitleAnimationController *TitleAnimation_Delete(TitleAnimationController *work)
{
    work->base.vtable = &data_ov006_0207b198;
    GameIrqTask_Disable(work->irq);
    GameAudio_StopMusic(0);
    while (GameAudio_GetMusicState() == 1) {}
    NNS_SndPlayerSetAllocatableChannel(0, 24565);
    if (work->irq) {
        GameIrqTask_DeleteSafe(work->irq);
        work->irq = 0;
    }
    if (work->archive) {
        DeleteTask(work->archive);
        work->archive = 0;
    }
    if (work->frame_task) {
        DeleteTask(work->frame_task);
        work->frame_task = 0;
    }
    TitleAnimation_ReleaseArchiveTables(work);
    TitleAnimation_ReleaseModelResources(work);
    TitleAnimation_ReleaseRenderState(work);
    TitleAnimation_ReleaseLocalizedResources(work);
    TitleAnimation_ReleaseSequence();
    data_0206032c.first_delay = work->previous_first_delay;
    data_0206032c.repeat_delay = work->previous_repeat_delay;
    GameTask_DestroyBase(&work->base);
    GameHeap_Delete(work);
    return work;
}

GameTask *TitleAnimationFrameTask_Delete(GameTask *task)
{
    task->vtable = &data_ov006_0207b38c;
    GameTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

void TitleAnimation_LoadArchiveTables(TitleAnimationController *work)
{
    for (int i = 0; i < 5; ++i) {
        u32 size = BattleArchive_GetDescriptorSize(work->archive, data_ov006_0207b038[i].descriptor);
        if (size) {
            work->offsets[i] = (u32 *)GameHeap_NewArray(4 * (size >> 2), 1, 0, 0);
            BattleArchive_Open(work->archive, (const u8 *)work->offsets[i], size, 0);
            int format = data_ov006_0207b038[i].table_format;
            if (format) {
                work->tables[i] = (u32 *)TitleAnimation_ReadArchiveEntry(work, i, 0, 0, 0, 1, 0);
                work->entries[i] = (u8 *)(work->tables[i] + 2);
                if (format == 1)
                    work->entry_data[i] = work->entries[i] + 20 * work->tables[i][0];
                else
                    work->entry_data[i] = work->entries[i] + 8 * work->tables[i][0];
            }
        }
    }
}

void TitleAnimation_LoadLocalizedResources(TitleAnimationController *work)
{
    /* Keep each resource pair in its native language branch. */
    if (!work->language) {
        work->localized_resource = TitleAnimation_ReadArchiveEntry(work, 4, 3, 0, 0, 1, 0);
        work->shared_resource = TitleAnimation_ReadArchiveEntry(work, 4, 4, 0, 0, 1, 0);
    } else {
        work->localized_resource = TitleAnimation_ReadArchiveEntry(work, 4, 8, 0, 0, 1, 0);
        work->shared_resource = TitleAnimation_ReadArchiveEntry(work, 4, 4, 0, 0, 1, 0);
    }
}

void TitleAnimation_ReleaseLocalizedResources(TitleAnimationController *work)
{
    if (work->localized_resource) {
        GameHeap_Free(work->localized_resource);
        work->localized_resource = 0;
    }
    if (work->shared_resource) {
        GameHeap_Free(work->shared_resource);
        work->shared_resource = 0;
    }
}

void TitleAnimation_ReleaseArchiveTables(TitleAnimationController *work)
{
    for (int i = 0; i < 5; ++i) {
        if (work->offsets[i]) {
            GameHeap_DeleteArray(work->offsets[i]);
            work->offsets[i] = 0;
        }
        if (work->tables[i]) {
            GameHeap_DeleteArray(work->tables[i]);
            work->tables[i] = 0;
        }
    }
}
}
