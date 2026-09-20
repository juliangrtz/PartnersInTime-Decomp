/* Repair an invalid save header, preserving the footer for a version-only reset.
 * The final eight-byte clear covers settings and the start of the directory. */
extern "C" {
#include <game/save_storage.h>
extern u8 data_0205e328;
extern u8 data_0205e32c[8192];
extern u8 data_0205e334[];
extern const SaveStorageSignature data_02049fb0;
int SaveStorage_Initialize(void) {
    int signature = SaveStorage_CheckSignature();
    int slot;
    if (signature <= 0) {
        if (signature < 0)
            MI_CpuFill8(data_0205e32c, 0, 8192);
        else
            MI_CpuFill8(data_0205e32c, 0, 8128);
        *(SaveStorageSignature *)data_0205e32c = data_02049fb0;
        SaveStorage_ClearDirectory();
        if (data_0205e328) {
            if (!SaveStorage_WriteSignature())
                return 0;
            if (!SaveStorage_WriteSettings())
                return 0;
            if (!SaveStorage_WriteDirectory())
                return 0;
            for (slot = 0; slot < 4; ++slot) {
                if (!SaveStorage_WriteSlot((u8)slot, (u8)slot, 0))
                    return 0;
                if (!SaveBackup_Wait())
                    return 0;
            }
            if (signature < 0)
                SaveStorage_WriteFooter(SAVE_FOOTER_ALL);
        }
    }
    if (!SaveStorage_CheckSettings())
        MI_CpuFill8(data_0205e334, 0, 8);
    return 1;
}
}
