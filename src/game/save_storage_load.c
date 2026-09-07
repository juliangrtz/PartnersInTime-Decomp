#include <game/save_storage.h>

extern u8 data_0205e328;
extern u8 data_0205e32c[8192];
extern int MATH_CalcChecksum16(const void *data, u32 size);

int SaveStorage_Load(void)
{
    data_0205e328 = 0;
    MI_CpuFill8(data_0205e32c, 0, 8192);
    SaveBackup_Init(1);
    SaveBackup_SetChecksum(MATH_CalcChecksum16);
    if (!SaveBackup_ReadAsync(0, data_0205e32c, 8192)) return 0;
    if (!SaveBackup_Wait()) return 0;
    data_0205e328 = 1;
    return 1;
}
