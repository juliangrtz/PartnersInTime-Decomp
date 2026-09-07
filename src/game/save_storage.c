#include <game/save_storage.h>

extern u8 data_0205e328;
extern u8 data_0205e32c[8192];
extern u8 data_0205e334[4];
extern u8 data_0205e338[20];
extern SaveStorageSlot data_0205e34c[4];
extern u8 data_020602ec[];
extern u8 data_020602fc[];
extern const SaveStorageSignature data_02049fb0;

int SaveStorage_Probe(void)
{
    u8 signature[8];
    if (!SaveBackup_ReadAsync(0, signature, 8)) return 0;
    if (!SaveBackup_Wait()) return 0;
    return 1;
}

int SaveStorage_Erase(void)
{
    if (!data_0205e328) return 0;
    MI_CpuFill8(data_0205e32c, 255, 8192);
    return SaveBackup_WriteAsync(0, data_0205e32c, 8192, 0) != 0;
}

int SaveStorage_CheckSignature(void)
{
    int i;
    const u8 *expected = (const u8 *)&data_02049fb0;
    const u8 *actual = data_0205e32c;
    for (i = 0; i < 6; ++i) {
        if (*actual != *expected) return -1;
        ++expected;
        ++actual;
    }
    return data_0205e32c[6] == 7;
}

int SaveStorage_WriteSignature(void)
{
    if (!SaveBackup_WriteAsync(0, data_0205e32c, 8, 0)) return 0;
    if (!SaveBackup_Wait()) return 0;
    return 1;
}

int SaveStorage_WriteSettings(void)
{
    if (!SaveBackup_WriteAsync(8, data_0205e334, 4, 1)) return 0;
    if (!SaveBackup_Wait()) return 0;
    return 1;
}

int SaveStorage_CheckSettings(void)
{
    return SaveBackup_CheckChecksum(data_0205e334, 4) != 0;
}

void SaveStorage_ClearDirectory(void)
{
    MI_CpuFill8(data_0205e338, 0, 20);
}

int SaveStorage_CheckDirectory(void)
{
    return SaveBackup_CheckChecksum(data_0205e338, 20) != 0;
}

int SaveStorage_WriteDirectory(void)
{
    if (!SaveBackup_WriteAsync(12, data_0205e338, 20, 1)) return 0;
    if (!SaveBackup_Wait()) return 0;
    return 1;
}

SaveStorageSlot *SaveStorage_GetSlot(u8 slot)
{
    return &data_0205e34c[slot];
}

void *SaveStorage_GetSlotExtra(u8 slot)
{
    return SaveStorage_GetSlot(slot)->extra;
}

int SaveStorage_WriteSlot(u8 source_slot, u8 destination_slot, int checksum)
{
    SaveStorageSlot *source = SaveStorage_GetSlot(source_slot);
    return SaveBackup_WriteAsync(2024 * destination_slot + 32, source, 2024, checksum) != 0;
}

u8 SaveStorage_GetState(void)
{
    return SaveBackup_GetState();
}

int SaveStorage_CheckSlot(u8 slot)
{
    return SaveBackup_CheckChecksum(SaveStorage_GetSlot(slot), 2024) != 0;
}

int SaveStorage_WriteFooter(enum SaveStorageFooter region)
{
    void *source;
    u32 size;
    u32 offset;
    if (!data_0205e328) return 0;
    /* The footer selector has the three values in SaveStorageFooter. */
    switch (region) {
    case SAVE_FOOTER_MAIN:
        source = data_020602ec;
        size = 16;
        offset = 8128;
        break;
    case SAVE_FOOTER_EXTRA:
        source = data_020602fc;
        offset = 8144;
        size = 32;
        break;
    case SAVE_FOOTER_ALL:
        source = data_020602ec;
        size = 64;
        offset = 8128;
        break;
    }
    if (!SaveBackup_WriteAsync(offset, source, size, 0)) return 0;
    if (!SaveBackup_Wait()) return 0;
    return 1;
}
