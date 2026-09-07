#include <game/session.h>
#include <game/heap.h>
#include <game/inventory.h>

extern GameSessionSettings data_02059fec;
extern GameSessionTask *data_02059ffc;
extern u8 data_02059f58;
extern u8 data_0204ffa8[];
extern void func_02007664(void);
extern u8 data_02059f7c[];
extern void MI_CpuFill8(void *destination, u8 value, u32 size);

void GameSession_Start(int language, u8 option_2, u8 option_3)
{
    GameSessionTask *task;
    GameSession_InitSettings(language, option_2, option_3, &data_02059fec);
    data_02059f58 = 0;
    func_02007664();
    task = GameHeap_New(sizeof(GameSessionTask), 1, data_0204ffa8, 0);
    if (task) task = GameSessionTask_Init(task, 8, (u32)data_0204ffa8, 0, 9, 0, &data_02059fec);
    data_02059ffc = task;
}

void GameSession_InitSettings(int language, u8 option_2, u8 option_3, GameSessionSettings *settings)
{
    int index;
    if (language == -1) {
        language = data_02059f7c[12];
        if (language != 1 && language != 2 && language != 3 && language != 4 && language != 5)
            language = 1;
    }
    settings->unknown_0 = 0;
    settings->unknown_1 = 0;
    settings->option_2 = option_2;
    settings->option_3 = option_3;
    settings->language = (u16)language;
    settings->unknown_9 = 1;
    settings->unknown_02 = 16;
    settings->unknown_04 = 15;
    for (index = 0; index < 2; ++index) settings->unknown_08[index] = 0;
}

void GameSession_ResetSaveState(void)
{
    GameSession_InitSettings(-1, 0, 1, &data_02059fec);
    /* Clear the script variable banks before resetting the party. */
    MI_CpuFill8(gSaveData, 0, 0x40);
    MI_CpuFill8(gSaveData + 0x40, 0, 8);
    MI_CpuFill8(gSaveData + 0x48, 0, 8);
    MI_CpuFill8(gSaveData + 0x50, 0, 0x80);
    MI_CpuFill8(gSaveData + 0xd0, 0, 0x120);
    MI_CpuFill8(gSaveData + 0x1f0, 0, 0x200);
    MI_CpuFill8(gSaveData + 0x3f0, 0, 8);
    gSaveData[0x516] = 0;
    gSaveData[0x517] = 0;
    *(u32 *)(gSaveData + 0x518) = 0;
    GameParty_Initialize(0);
}
