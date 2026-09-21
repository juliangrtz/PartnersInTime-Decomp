/*
 * Scene object captions (overlay 7, 0x02083C20-0x02083E6C).
 * Render two lines into the pause workspace, then queue their OBJ tile uploads.
 * The Scene VM retains five arguments; only mode, table and entry are consumed.
 */
#include "pause_scene_internal.h"
extern "C" {
#include <game/text.h>
#include <game/scene_script.h>
extern const u32 *const data_ov007_02090620[];
void MIi_CpuClear16(u16, void *, u32);
void func_ov005_020663d8(int);
void SceneSpriteTasks_Create(int, int);
}
struct SceneTextTileTask {
    u8 prefix[40];
    int screen, offset, row, column;
    u8 tail[16];
};
extern "C" SceneTextTileTask *SceneText_QueueTileUpload(int, int, int, int);
extern "C" void func_ov007_02083e6c(SceneTextTileTask *);
typedef char SceneTextTileTaskSize[sizeof(SceneTextTileTask) == 72 ? 1 : -1];

/* The native clear wrapper stores and reloads its halfword fill value. */
static inline void ClearTextScratch(void)
{
    volatile u16 zero = 0;
    MIi_CpuClear16(zero, WORK.number_scratch, 4096);
}
extern "C" SceneTextTileTask *SceneText_QueueTileUpload(int screen, int offset, int row, int column)
{
    SceneTextTileTask *task = (SceneTextTileTask *)func_ov005_0206659c(
        (void (*)(PauseMenuElement *))func_ov007_02083e6c, 11, 1);
    task->screen = screen;
    task->offset = offset;
    task->row = row;
    task->column = column;
    return task;
}

extern "C" void SceneText_RenderObjectTiles(SceneObject *, int mode, int table, int entry, int)
{
    if (mode != 0)
        return;
    GameText text;
    GameText_Init(&text, data_ov007_02090620, WORK.number_scratch, 0,
        0, 0, 1, 3, 1, 6, 31, 0, 255, 0);
    text.text = (const u8 *)GameTextResources_GetEntry((u16)table, (u16)entry);
    func_ov005_020663d8(6);
    ClearTextScratch();
    text.cursor.bits.x = 0;
    text.cursor.bits.y = 0;
    /* Each line occupies eight queued 32-pixel sprite strips. */
    int line = 0;
    do {
        for (;;) {
            if (*text.text == 0)
                break;
            if (text.text[0] == 255 && text.text[1] == 0) {
                text.cursor.bits.x = 0;
                text.cursor.bits.y += 16;
                text.text += 2;
                break;
            }
            GameText_Next(&text, 0, 0);
        }
        for (int segment = 0; segment < 8; ++segment)
            SceneText_QueueTileUpload(1, 640, line, segment);
        /* The first row spans the screen; center the second on its text. */
        if (line == 0)
            SceneSpriteTasks_Create(256, line);
        else
            SceneSpriteTasks_Create(text.cursor.bits.x, line);
        ++line;
    } while (line < 2);
    *(u16 *)(data_ov007_020a6b90 + 0xA7EA) = 1;
}
