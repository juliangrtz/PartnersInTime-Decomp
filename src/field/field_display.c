#include <game/field_display.h>
#include <game/heap.h>
#include <nitro/gx_init.h>

/* View of the persistent state through the two palette backup pointers. */
typedef struct FieldPaletteBackupState {
    u8 unknown_00[48];
    FieldPaletteBackup *main, *sub;
} FieldPaletteBackupState;
typedef char FieldPaletteBackupState_SizeCheck[sizeof(FieldPaletteBackupState) == 56 ? 1 : -1];
extern FieldPaletteBackupState data_0205a01c;
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
extern int func_02035850(void), func_0203582c(void), func_020357dc(void), func_020357b4(void);
#define REG16(address) (*(vu16 *)(address))
#define REG32(address) (*(vu32 *)(address))
extern void func_02037190(void), func_02036e28(void), func_02037024(void);
extern void func_02036f24(int, int, int, int);
extern void func_02036ee0(int, int, int, int, int);
extern void func_02035aec(int), func_02035610(void), func_0203613c(int), func_02035fd0(int);
extern void func_02035ebc(int), func_02035e04(int), func_02035a40(int), func_020359c4(int);
extern void func_02035938(int), func_020358ac(int);
extern void MIi_CpuClearFast(u32, void *, u32);
static inline void CopyPalette(const void *source, void *destination, u32 size)
{
    if (source < destination)
        func_0202cd2c(source, destination, size);
    else
        func_0202cc58(source, destination, size);
}
static inline void ClearFast(void *destination, u32 bytes)
{
    volatile u32 zero = 0;
    MIi_CpuClearFast(zero, destination, bytes);
}
void FieldDisplay_SavePalettes(void)
{
    int bank;
    data_0205a01c.main = GameHeap_NewArray(sizeof(FieldPaletteBackup), 2, 0, 0);
    CopyPalette((const void *)0x05000000, data_0205a01c.main->background, 512);
    CopyPalette((const void *)0x05000200, data_0205a01c.main->objects, 512);
    bank = func_02035850();
    CopyPalette((const void *)0x06882000, data_0205a01c.main->extended_background[0], 512);
    CopyPalette((const void *)0x06884000, data_0205a01c.main->extended_background[1], 512);
    CopyPalette((const void *)0x06886000, data_0205a01c.main->extended_background[2], 512);
    func_02035ebc(bank);
    bank = func_0203582c();
    CopyPalette((const void *)0x06890000, data_0205a01c.main->extended_objects, 8192);
    func_02035e04(bank);
    data_0205a01c.sub = GameHeap_NewArray(sizeof(FieldPaletteBackup), 2, 0, 0);
    CopyPalette((const void *)0x05000400, data_0205a01c.sub->background, 512);
    CopyPalette((const void *)0x05000600, data_0205a01c.sub->objects, 512);
    bank = func_020357dc();
    CopyPalette((const void *)0x0689A000, data_0205a01c.sub->extended_background[0], 512);
    CopyPalette((const void *)0x0689C000, data_0205a01c.sub->extended_background[1], 512);
    CopyPalette((const void *)0x0689E000, data_0205a01c.sub->extended_background[2], 512);
    func_02035938(bank);
    bank = func_020357b4();
    CopyPalette((const void *)0x068A0000, data_0205a01c.sub->extended_objects, 8192);
    func_020358ac(bank);
}

void FieldDisplay_InitializeVram(void *unused)
{
    REG16(0x04000304) &= ~0x8000;
    func_02035aec(511);
    ClearFast((void *)0x06800000, 0xA4000);
    func_02035610();
    ClearFast((void *)0x05000000, 0x800);
    func_0203613c(1);
    func_02035fd0(2);
    func_02035ebc(16);
    func_02035e04(32);
    GX_SetGraphicsMode(1, 0, 0);
    REG32(0x04000000) = (REG32(0x04000000) & 0xFFCFFFEF) | 0x200010;
    func_02035a40(4);
    func_020359c4(8);
    func_02035938(128);
    func_020358ac(256);
    GXS_SetGraphicsMode(0);
    REG32(0x04001000) = (REG32(0x04001000) & 0xFFCFFFEF) | 0x200010;
}
void FieldDisplay_InitializeGeometry(void *unused)
{
    func_02037190();
    func_02036e28();
    func_02037024();
    /* The native initialization submits the swap command twice. */
    REG32(0x04000540) = 1;
    REG32(0x04000540) = 1;
    REG16(0x04000060) &= ~0x3002;
    REG16(0x04000060) &= 0xCFFB;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 8;
    REG16(0x04000060) &= 0xCFEF;
    REG16(0x04000060) &= 0xCFDF;
    func_02036f24(0, 0, 0, 0);
    func_02036ee0(0, 0, 0x7FFF, 63, 0);
    REG32(0x04000580) = 0xBFFF0000;
}
