extern "C" {
#include <game/task.h>
#include <game/heap.h>
}

extern "C" {

extern u32 data_ov005_0206a170;
extern int data_ov005_0206a174;
extern u32 data_ov005_0206a178;
extern void *data_ov005_02069f28[], *data_ov005_02069f3c[];

extern void GameFrame_WaitVBlank(void);
extern void GameInput_Read(void);
extern void func_02009094(int screen);
extern void func_ov005_02068594(void);
extern void func_ov005_0206642c(int first, int last);
extern void func_ov005_02069cb8(void);

GameTask *func_ov005_02065dd4(GameTask *task, u32 priority, u32 unused);
GameTask *func_ov005_02065eb4(GameTask *task, u32 priority, u32 unused);

u32 func_ov005_02065f00(void) {
    return data_ov005_0206a170;
}

void func_ov005_02065eec(void) {
    data_ov005_0206a170 = 0;
}

u32 func_ov005_02065edc(void) {
    return data_ov005_0206a178;
}

GameTask *func_ov005_02065eb4(GameTask *task, u32 priority, u32 unused) {
    GameTask_Init(task, priority, unused, 0);
    task->vtable = (GameTaskVTable *)data_ov005_02069f28;
    return task;
}

GameTask *func_ov005_02065e90(GameTask *task) {
    task->vtable = (GameTaskVTable *)data_ov005_02069f28;
    GameTask_DestroyBase(task);
    return task;
}

GameTask *func_ov005_02065e64(GameTask *task) {
    task->vtable = (GameTaskVTable *)data_ov005_02069f28;
    GameTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

void func_ov005_02065dfc(void) {
    GameFrame_WaitVBlank();
    data_ov005_0206a170 = 0;
    data_ov005_0206a178 = 0;
    GameInput_Read();
    if (data_ov005_0206a174) func_ov005_02068594();
    func_ov005_0206642c(0, 10);
    data_ov005_0206a178 = 1;
}

GameTask *func_ov005_02065dd4(GameTask *task, u32 priority, u32 unused) {
    GameTask_Init(task, priority, unused, 0);
    task->vtable = (GameTaskVTable *)data_ov005_02069f3c;
    return task;
}

GameTask *func_ov005_02065db0(GameTask *task) {
    task->vtable = (GameTaskVTable *)data_ov005_02069f3c;
    GameTask_DestroyBase(task);
    return task;
}

GameTask *func_ov005_02065d84(GameTask *task) {
    task->vtable = (GameTaskVTable *)data_ov005_02069f3c;
    GameTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

void func_ov005_02065d40(void) {
    if (!data_ov005_0206a178) return;
    func_ov005_02069cb8();
    data_ov005_0206a170 = 1;
}

}
