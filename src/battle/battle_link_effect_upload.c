#include <nitro.h>
void func_02038108(const void *, u32, u32);
void BattleLinkEffect_UploadTiles(void) {
    func_02038108((const void *)0x06898000, 8192, 24576);
    *(volatile u32 *)0x04001010 = 0;
}
