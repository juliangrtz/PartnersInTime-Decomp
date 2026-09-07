#ifndef NITRO_GX_INIT_H
#define NITRO_GX_INIT_H

#include <nitro.h>

void GX_Init(void);
void GX_SetVCountEqVal(int line);
int GX_HBlankIntr(int enable);
int GX_VBlankIntr(int enable);
void GX_DispOff(void);
void GX_DispOn(void);
void GX_SetGraphicsMode(int display, int background, int background0_3d);
void GXS_SetGraphicsMode(int background);
void GXi_SetMasterBrightness(vu16 *register_address, int brightness);
void func_020350d0(void);

#endif
