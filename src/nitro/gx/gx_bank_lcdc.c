#include <nitro/gx_vram.h>

extern u16 data_02062c58;
extern u16 data_02062c5a;
extern u16 data_02062c5e;
extern u16 data_02062c60;
extern u16 data_02062c66;
extern u16 data_02062c68;

void func_020363f4(u32 bank_mask);

/* Move an engine's bank assignment back into LCDC address space. */
u32 func_02035874(u16 *assignment) {
    u32 bank_mask = *assignment;
    *assignment = 0;
    data_02062c50.lcdc |= bank_mask;
    func_020363f4(bank_mask);
    return bank_mask;
}

u32 func_02035850(void) {
    *(vu32 *)0x04000000 &= ~0x40000000;
    return func_02035874(&data_02062c5e);
}

u32 func_0203582c(void) {
    *(vu32 *)0x04000000 &= ~0x80000000;
    return func_02035874(&data_02062c60);
}

u32 func_02035818(void) {
    return func_02035874(&data_02062c58);
}

u32 func_02035804(void) {
    return func_02035874(&data_02062c5a);
}

u32 func_020357dc(void) {
    *(vu32 *)0x04001000 &= ~0x40000000;
    return func_02035874(&data_02062c66);
}

u32 func_020357b4(void) {
    *(vu32 *)0x04001000 &= ~0x80000000;
    return func_02035874(&data_02062c68);
}
