extern "C" {
#include <game/window.h>
#include <game/scene_window.h>
extern u8 *data_ov007_020a6b90;
extern u8 data_ov005_0206a180[];
extern void func_ov005_02067424(void *, u8);
extern void func_ov005_02066ed4(void *, u8, int, void **, int);
extern int func_ov005_02067238(void *, u8, u16);
extern u32 OS_DisableIrqMask(u32), OS_EnableIrqMask(u32);
}
struct SceneWindowState {
    GameWindowManager *manager;
    u32 *archive;
    void *fonts;
    void *text[20];
};
struct SceneWindowLoadTaskPrefix {
    u8 unknown00[40];
    int entry;
};
typedef char SceneWindowStateSizeCheck[sizeof(SceneWindowState) == 92 ? 1 : -1];
typedef char SceneWindowLoadTaskPrefixSizeCheck[sizeof(SceneWindowLoadTaskPrefix) == 44 ? 1 : -1];
#define STATE ((SceneWindowState *)data_ov007_020a6b90)
#define ARCHIVE (*(void **)(data_ov005_0206a180 + 44))
extern "C" {
extern SceneWindowLoadTaskPrefix *func_ov005_0206659c(void (*)(SceneWindowLoadTaskPrefix *), int, int);
extern void func_ov005_0206650c(SceneWindowLoadTaskPrefix *);
/* Decodes the archive into independent text and font buffers. */
void func_ov007_02089f34(void);
void *SceneText_GetEntry(const u32 *, int);
u32 SceneText_GetEntrySize(const u32 *, int);
void SceneText_UpdateLoad(SceneWindowLoadTaskPrefix *);
}
/* Preserve the null check retained by the original inlined delete expression. */
static inline void DeleteWindow(GameWindowManager *manager)
{
    if (manager)
        ((GameTaskDispatch *)manager)->delete_task();
}

extern "C" {
void SceneWindow_Destroy(void)
{
    u32 irq = OS_DisableIrqMask(1);
    GameSpriteAnimator *animator;
    int i;
    if (STATE->manager) {
        animator = STATE->manager->animator;
        GameSpriteAllocation_Unlink(&animator->allocation[0]);
        animator->state.bits.main_allocated = 0;
        animator = STATE->manager->animator;
        GameSpriteAllocation_Unlink(&animator->allocation[1]);
        animator->state.bits.sub_allocated = 0;
    }
    if (STATE->manager) {
        DeleteWindow(STATE->manager);
        STATE->manager = 0;
    }
    OS_EnableIrqMask(irq);
    if (STATE->archive) {
        GameHeap_DeleteArray(STATE->archive);
        STATE->archive = 0;
    }
    if (STATE->fonts) {
        GameHeap_DeleteArray(STATE->fonts);
        STATE->fonts = 0;
    }
    for (i = 1; i < 20; ++i) {
        if (STATE->text[i]) {
            GameHeap_DeleteArray(STATE->text[i]);
            STATE->text[i] = 0;
        }
    }
    func_ov005_02067424(ARCHIVE, 5);
}

u32 SceneText_GetEntrySize(const u32 *archive, int index)
{
    u32 size = 0;
    if (archive)
        size = (archive + index)[1] - archive[index];
    return size;
}

void SceneText_UpdateLoad(SceneWindowLoadTaskPrefix *task)
{
    if (func_ov005_02067238(ARCHIVE, 5, (u16)task->entry)) {
        func_ov007_02089f34();
        func_ov005_0206650c(task);
    }
}

void SceneText_LoadArchive(int entry)
{
    SceneWindowLoadTaskPrefix *task;
    if (STATE->archive) {
        GameHeap_DeleteArray(STATE->archive);
        STATE->archive = 0;
    }
    if (STATE->fonts) {
        GameHeap_DeleteArray(STATE->fonts);
        STATE->fonts = 0;
    }
    func_ov005_02066ed4(ARCHIVE, 5, entry, (void **)&STATE->archive, 1);
    task = func_ov005_0206659c(SceneText_UpdateLoad, 5, 1);
    task->entry = entry;
}

int SceneText_IsLoadComplete(void)
{
    return STATE->archive == 0;
}

void *SceneText_GetEntry(const u32 *archive, int index)
{
    void *entry = 0;
    if (archive && SceneText_GetEntrySize(archive, index))
        entry = (u8 *)archive + archive[index];
    return entry;
}
}
