#include "save_menu_internal.h"

void SaveScene_InitGeometry(SaveSceneTask *unused)
{
    func_02037190();
    func_02037024();
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 0x10;
    REG16(0x04000060) &= 0xCFFB;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 8;
    func_02036ee0(0, 0, 0x7FFF, 63, 0);
    REG32(0x04000540) = 0;
    REG32(0x04000580) = 0xBFFF0000;
    func_02037108();
    REG32(0x04000440) = 0;
    REG32(0x04000454) = 0;
    G3i_OrthoW(0, 192 * 4096, 0, 256 * 4096, -1024 * 4096, 1024 * 4096, 4096, 1, 0);
    REG32(0x0400044C) = 0;
}
