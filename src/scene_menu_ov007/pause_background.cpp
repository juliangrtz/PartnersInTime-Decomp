#include "pause_scene_internal.h"
#include <game/pause_background.h>
extern "C" void func_ov005_0206650c(PauseBackgroundTask *);
static inline PauseBackgroundTask *CreateBackgroundTask(void (*callback)(PauseBackgroundTask *), int group)
{
    return (PauseBackgroundTask *)func_ov005_0206659c((void (*)(PauseMenuElement *))callback, group, 1);
}
extern "C" {
extern u8 gSceneMenuSubmenus[];
extern u8 data_ov007_0208da28[], data_ov007_0208da29[], data_ov007_0208da2a[], data_ov007_0208da2b[],
    data_ov007_0208da2c[];
void *Overlay5Archive_ReadEntry(Overlay5Archive *, int, int, int, int);
void *func_ov005_02066f78(Overlay5Archive *, int, int, int, int);
int func_ov005_02067238(Overlay5Archive *, int, int);
void func_ov005_02066ed4(Overlay5Archive *, int, int, void **, int);
void MI_UncompressLZ16(const void *, void *);
void GameResource_Move16(const void *, void *, u32);
void func_ov007_02070938(const u16 *, int);
}

extern "C" void PauseBackground_LoadPage(PauseBackgroundTask *task)
{
    switch (task->state) {
    case 0: {
        void *palette = Overlay5Archive_ReadEntry(ARCHIVE, 1, data_ov007_0208da2c[5 * task->selection], 0, 0);
        PauseBackground_QueuePalette(palette, 3);
        func_ov005_02066ed4(ARCHIVE, 1, data_ov007_0208da2a[5 * task->selection], &task->buffer, 0);
        ++task->state;
        break;
    }
    case 1:
        if (func_ov005_02067238(ARCHIVE, 1, data_ov007_0208da2a[5 * task->selection])) {
            Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_MAIN, 2, task->buffer, 0);
            if (task->buffer) {
                GameHeap_DeleteArray(task->buffer);
                task->buffer = 0;
            }
            void *map = func_ov005_02066f78(ARCHIVE, 1, data_ov007_0208da2b[5 * task->selection], 0, 0);
            func_ov007_02070938((const u16 *)map, 0);
            if (map)
                GameHeap_DeleteArray(map);
            Overlay5DisplayBg_LoadScreen(DISPLAY_ENGINE_MAIN, 2, WORK.secondary_background_maps[0], 0, 2048);
            if (task->parent->pending)
                --task->parent->pending;
            ++task->state;
        }
        break;
    case 2:
        func_ov005_02066ed4(ARCHIVE, 1, data_ov007_0208da28[5 * task->selection], &task->buffer, 0);
        ++task->state;
        break;
    case 3:
        if (func_ov005_02067238(ARCHIVE, 1, data_ov007_0208da28[5 * task->selection])) {
            MI_UncompressLZ16(task->buffer, WORK.background_tiles[0]);
            Overlay5DisplayBg_LoadCharacters(DISPLAY_ENGINE_MAIN, 1, WORK.background_tiles[0], 0, 32768);
            if (task->buffer) {
                GameHeap_DeleteArray(task->buffer);
                task->buffer = 0;
            }
            ++task->state;
        }
        break;
    case 4: {
        void *map = Overlay5Archive_ReadEntry(ARCHIVE, 1, data_ov007_0208da29[5 * task->selection], 0, 0);
        MI_UncompressLZ16(map, WORK.background_maps[0]);
        if (map)
            GameHeap_DeleteArray(map);
        u16 *tiles;
        int i = 0;
        tiles = (u16 *)WORK.background_maps[0];
        for (; i < 1024; ++i) {
            *tiles |= 0x3000;
            ++tiles;
        }
        Overlay5DisplayBg_LoadScreen(DISPLAY_ENGINE_MAIN, 1, WORK.background_maps[0], 0, 2048);
        func_ov005_0206650c(task);
        break;
    }
    }
}
extern "C" void PauseBackground_LoadDirect(PauseBackgroundTask *task)
{
    switch (task->state) {
    case 0: {
        void *palette = Overlay5Archive_ReadEntry(ARCHIVE, 1, data_ov007_0208da2c[5 * task->selection], 0, 0);
        PauseBackground_QueuePalette(palette, 4);
        func_ov005_02066ed4(ARCHIVE, 1, data_ov007_0208da2a[5 * task->selection], &task->buffer, 0);
        ++task->state;
        break;
    }
    case 1:
        if (func_ov005_02067238(ARCHIVE, 1, data_ov007_0208da2a[5 * task->selection])) {
            MI_UncompressLZ16(task->buffer, (void *)0x06018000);
            if (task->buffer) {
                GameHeap_DeleteArray(task->buffer);
                task->buffer = 0;
            }
            void *map = func_ov005_02066f78(ARCHIVE, 1, data_ov007_0208da2b[5 * task->selection], 0, 0);
            func_ov007_02070938((const u16 *)map, 1);
            if (map)
                GameHeap_DeleteArray(map);
            GameResource_Move16(WORK.secondary_background_maps[1], (void *)0x06002000, 2048);
            if (task->parent->selection)
                --task->parent->selection;
            func_ov005_0206650c(task);
        }
        break;
    }
}
extern "C" void PauseBackground_LoadBuffered(PauseBackgroundTask *task)
{
    switch (task->state) {
    case 0: {
        void *palette = Overlay5Archive_ReadEntry(ARCHIVE, 1, data_ov007_0208da2c[5 * task->selection], 0, 0);
        PauseBackground_QueuePalette(palette, 4);
        func_ov005_02066ed4(ARCHIVE, 1, data_ov007_0208da28[5 * task->selection], &task->buffer, 0);
        ++task->state;
        break;
    }
    case 1:
        if (func_ov005_02067238(ARCHIVE, 1, data_ov007_0208da28[5 * task->selection])) {
            MI_UncompressLZ16(task->buffer, WORK.background_tiles[1]);
            if (task->buffer) {
                GameHeap_DeleteArray(task->buffer);
                task->buffer = 0;
            }
            void *map = Overlay5Archive_ReadEntry(ARCHIVE, 1, data_ov007_0208da29[5 * task->selection], 0, 0);
            MI_UncompressLZ16(map, WORK.background_maps[1]);
            if (map)
                GameHeap_DeleteArray(map);
            u16 *tiles;
            int i = 0;
            tiles = (u16 *)WORK.background_maps[1];
            for (; i < 1024; ++i) {
                *tiles |= 0x4000;
                ++tiles;
            }
            if (task->parent->selection)
                --task->parent->selection;
            func_ov005_0206650c(task);
        }
        break;
    }
}
extern "C" void PauseBackground_StartPageLoad(PauseBackgroundTask *parent)
{
    PauseBackgroundTask *task = CreateBackgroundTask(PauseBackground_LoadPage, 5);
    switch (WORK.selected_menu) {
    case 0:
        if (!gSceneMenuSubmenus[WORK.selected_menu])
            task->selection = 0;
        else
            task->selection = 1;
        break;
    case 1:
        if (WORK.special_available)
            task->selection = 3;
        else
            task->selection = 2;
        break;
    case 2:
        if (WORK.special_available)
            task->selection = 6;
        else
            task->selection = 5;
        break;
    case 3:
        task->selection = 7;
        break;
    case 4:
        task->selection = 8;
        break;
    }
    task->parent = parent;
    parent->pending = 1;
}
extern "C" void PauseBackground_StartAlternateLoad(PauseBackgroundTask *parent, int selection, int buffered)
{
    PauseBackgroundTask *task;
    if (buffered)
        task = CreateBackgroundTask(PauseBackground_LoadBuffered, 5);
    else
        task = CreateBackgroundTask(PauseBackground_LoadDirect, 5);
    task->selection = selection;
    task->parent = parent;
    parent->selection = 1;
}
extern "C" void PauseBackground_Upload(int alternate)
{
    if (!alternate) {
        Overlay5DisplayBg_LoadCharacters(DISPLAY_ENGINE_MAIN, 1, WORK.background_tiles[0], 0, 32768);
        Overlay5DisplayBg_LoadScreen(DISPLAY_ENGINE_MAIN, 1, WORK.background_maps[0], 0, 2048);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 3);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 2, 4);
    } else {
        Overlay5DisplayBg_LoadCharacters(DISPLAY_ENGINE_MAIN, 1, WORK.background_tiles[1], 0, 32768);
        Overlay5DisplayBg_LoadScreen(DISPLAY_ENGINE_MAIN, 1, WORK.background_maps[1], 0, 2048);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 4);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 2, 6);
    }
    WORK.active_background = alternate;
}
extern "C" void PauseBackground_SelectBank(int alternate)
{
    if (!alternate) {
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 1, 3);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 1, 4);
    } else {
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 1, 4);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 1, 6);
    }
}
extern "C" void PauseBackground_ApplyUpload(PauseBackgroundTask *task)
{
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 1, 1);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 1, 2);
    if (!task->selection) {
        Overlay5DisplayBg_LoadCharacters(DISPLAY_ENGINE_MAIN, 1, WORK.background_tiles[0], 0, 32768);
        Overlay5DisplayBg_LoadScreen(DISPLAY_ENGINE_MAIN, 1, WORK.background_maps[0], 0, 2048);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 3);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 2, 4);
    } else {
        Overlay5DisplayBg_LoadCharacters(DISPLAY_ENGINE_MAIN, 1, WORK.background_tiles[1], 0, 32768);
        Overlay5DisplayBg_LoadScreen(DISPLAY_ENGINE_MAIN, 1, WORK.background_maps[1], 0, 2048);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 4);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 2, 6);
    }
    WORK.active_background = task->selection;
    REG16(0x04000052) = 16;
    func_ov005_0206650c(task);
}
extern "C" PauseBackgroundTask *PauseBackground_QueueUpload(int alternate)
{
    PauseBackgroundTask *task = CreateBackgroundTask(PauseBackground_ApplyUpload, 11);
    task->selection = alternate;
    return task;
}
extern "C" void PauseBackground_ApplyBankSwap(PauseBackgroundTask *task)
{
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 1, 1);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 1, 2);
    if (!task->selection) {
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 4);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 2, 6);
    } else {
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 3);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 2, 4);
    }
    REG16(0x04000052) = 16;
    func_ov005_0206650c(task);
}
extern "C" PauseBackgroundTask *PauseBackground_QueueBankSwap(int alternate)
{
    PauseBackgroundTask *task = CreateBackgroundTask(PauseBackground_ApplyBankSwap, 11);
    task->selection = alternate;
    return task;
}
