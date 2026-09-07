#include <game/input.h>
#include <game/system.h>
#include <game/heap.h>
#include <game/archive_io.h>
#include <game/save_storage.h>
#include <game/rumble.h>
#include <nitro/tp.h>
#include <nitro/rtc.h>
#include <nitro/os_owner.h>

extern GameInput data_02059f68;
extern void *data_02059f60;
extern u8 data_0204ffdc[];
extern TpCalibration data_02059f80;
extern OsOwnerInfo data_02059f88;
extern u8 data_02059fd8[6];
extern void MI_CpuFill8(void *, u8, u32);
extern void Random_SetSeed(u32);
extern void func_02008fc4(void);

void GameSystem_Init(void)
{
    u32 table_size;
    RtcTime time;
    MI_CpuFill8(&data_02059f68, 0, sizeof(data_02059f68));
    data_02059f68.first_delay = 10;
    data_02059f68.repeat_delay = 2;
    GameRumble_InitControl();
    SaveStorage_Load();
    ArchiveIO_InitFilesystem(-1, 0, 0);
    table_size = ArchiveIO_GetFileTableSize();
    data_02059f60 = GameHeap_NewArray(table_size, 1, data_0204ffdc, 0);
    ArchiveIO_LoadFileTables(data_02059f60, table_size);
    TP_Init();
    data_02059f7c.calibrated_touch = TP_GetUserInfo(&data_02059f80);
    if (data_02059f7c.calibrated_touch) TP_SetCalibrateParam(&data_02059f80);
    OS_GetOwnerInfo(&data_02059f88);
    OS_GetMacAddress(data_02059fd8);
    *(u64 *)data_02059f7c.owner_value_68 = OS_GetOwnerValue68();
    RTC_GetTime(&time);
    Random_SetSeed((time.hour << 16) + (time.minute << 8) + time.second);
    func_02008fc4();
}
