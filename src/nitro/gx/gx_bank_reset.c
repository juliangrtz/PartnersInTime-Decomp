#include <nitro/gx_vram.h>

extern u16 data_02062c48;
extern u16 data_02062c52;
extern u16 data_02062c54;
extern u16 data_02062c58;
extern u16 data_02062c5a;
extern u16 data_02062c5c;
extern u16 data_02062c5e;
extern u16 data_02062c60;
extern u16 data_02062c62;
extern u16 data_02062c64;
extern u16 data_02062c66;
extern u16 data_02062c68;

void func_0203b09c(u16 bank_mask, u16 lock_id);

/* Detach the selected banks, disable their mappings, and release the lock. */
u32 func_020356d0(u16 *assignment) {
    u32 bank_mask = *assignment;
    *assignment = 0;
    if (bank_mask & 0x001) *(vu8 *)0x04000240 = 0;
    if (bank_mask & 0x002) *(vu8 *)0x04000241 = 0;
    if (bank_mask & 0x004) *(vu8 *)0x04000242 = 0;
    if (bank_mask & 0x008) *(vu8 *)0x04000243 = 0;
    if (bank_mask & 0x010) *(vu8 *)0x04000244 = 0;
    if (bank_mask & 0x020) *(vu8 *)0x04000245 = 0;
    if (bank_mask & 0x040) *(vu8 *)0x04000246 = 0;
    if (bank_mask & 0x080) *(vu8 *)0x04000248 = 0;
    if (bank_mask & 0x100) *(vu8 *)0x04000249 = 0;
    func_0203b09c(bank_mask, data_02062c48);
    return bank_mask;
}

u32 GX_ResetBankForBG(void) {
    return func_020356d0(&data_02062c52);
}

u32 GX_ResetBankForOBJ(void) {
    return func_020356d0(&data_02062c54);
}

u32 GX_ResetBankForBGExtPltt(void) {
    *(vu32 *)0x04000000 &= ~0x40000000;
    return func_020356d0(&data_02062c5e);
}

u32 GX_ResetBankForOBJExtPltt(void) {
    *(vu32 *)0x04000000 &= ~0x80000000;
    return func_020356d0(&data_02062c60);
}

u32 GX_ResetBankForTex(void) {
    return func_020356d0(&data_02062c58);
}

u32 GX_ResetBankForTexPltt(void) {
    return func_020356d0(&data_02062c5a);
}

u32 GX_ResetBankForClearImage(void) {
    return func_020356d0(&data_02062c5c);
}

u32 func_02035610(void) {
    return func_020356d0(&data_02062c50.lcdc);
}

u32 GX_ResetBankForSubBG(void) {
    return func_020356d0(&data_02062c62);
}

u32 GX_ResetBankForSubOBJ(void) {
    return func_020356d0(&data_02062c64);
}

u32 GX_ResetBankForSubBGExtPltt(void) {
    *(vu32 *)0x04001000 &= ~0x40000000;
    return func_020356d0(&data_02062c66);
}

u32 GX_ResetBankForSubOBJExtPltt(void) {
    *(vu32 *)0x04001000 &= ~0x80000000;
    return func_020356d0(&data_02062c68);
}
