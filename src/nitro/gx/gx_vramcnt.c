#include <nitro/gx_vram.h>

#define VRAM_A (*(vu8 *)0x04000240)
#define VRAM_B (*(vu8 *)0x04000241)
#define VRAM_C (*(vu8 *)0x04000242)
#define VRAM_D (*(vu8 *)0x04000243)
#define VRAM_E (*(vu8 *)0x04000244)
#define VRAM_F (*(vu8 *)0x04000245)
#define VRAM_G (*(vu8 *)0x04000246)
#define VRAM_H (*(vu8 *)0x04000248)
#define VRAM_I (*(vu8 *)0x04000249)
#define DISPLAY_MAIN (*(vu32 *)0x04000000)
#define DISPLAY_SUB (*(vu32 *)0x04001000)

/* Return the previous assignment to LCDC, then claim the requested banks. */
#define REASSIGN_BANKS(field, bank_mask) \
    do { \
        data_02062c50.lcdc = \
            (data_02062c50.lcdc | data_02062c50.field) & ~bank_mask; \
        data_02062c50.field = bank_mask; \
    } while (0)

void func_020363f4(u32 bank_mask) {
    if (bank_mask & 0x001) VRAM_A = 0x80;
    if (bank_mask & 0x002) VRAM_B = 0x80;
    if (bank_mask & 0x004) VRAM_C = 0x80;
    if (bank_mask & 0x008) VRAM_D = 0x80;
    if (bank_mask & 0x010) VRAM_E = 0x80;
    if (bank_mask & 0x020) VRAM_F = 0x80;
    if (bank_mask & 0x040) VRAM_G = 0x80;
    if (bank_mask & 0x080) VRAM_H = 0x80;
    if (bank_mask & 0x100) VRAM_I = 0x80;
}

/* Main-engine background mappings, with the original shared suffixes. */
void func_0203613c(int bank_mask) {
    REASSIGN_BANKS(bg, bank_mask);
    switch (bank_mask) {
    case 8: VRAM_D = 0x81; break;
    case 12: VRAM_D = 0x89;
    case 4: VRAM_C = 0x81; break;
    case 14: VRAM_D = 0x91;
    case 6: VRAM_C = 0x89;
    case 2: VRAM_B = 0x81; break;
    case 15: VRAM_D = 0x99;
    case 7: VRAM_C = 0x91;
    case 3: VRAM_B = 0x89;
    case 1: VRAM_A = 0x81; break;
    case 11: VRAM_A = 0x81; VRAM_B = 0x89; VRAM_D = 0x91; break;
    case 13: VRAM_D = 0x91;
    case 5: VRAM_A = 0x81; VRAM_C = 0x89; break;
    case 9: VRAM_A = 0x81; VRAM_D = 0x89; break;
    case 10: VRAM_B = 0x81; VRAM_D = 0x89; break;
    case 112: VRAM_G = 0x99;
    case 48: VRAM_F = 0x91;
    case 16: VRAM_E = 0x81; break;
    case 80: VRAM_G = 0x91; VRAM_E = 0x81; break;
    case 96: VRAM_G = 0x89;
    case 32: VRAM_F = 0x81; break;
    case 64: VRAM_G = 0x81; break;
    case 0: break;
    }
    func_020363f4(data_02062c50.lcdc);
}

/* Main-engine object mappings. */
void func_02035fd0(int bank_mask) {
    REASSIGN_BANKS(obj, bank_mask);
    switch (bank_mask) {
    case 3: VRAM_B = 0x8A;
    case 1: VRAM_A = 0x82; break;
    case 2: VRAM_B = 0x82; break;
    case 112: VRAM_G = 0x9A;
    case 48: VRAM_F = 0x92;
    case 16: VRAM_E = 0x82; break;
    case 80: VRAM_G = 0x92; VRAM_E = 0x82; break;
    case 96: VRAM_G = 0x8A;
    case 32: VRAM_F = 0x82; break;
    case 64: VRAM_G = 0x82; break;
    case 0: break;
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_02035ebc(int bank_mask) {
    REASSIGN_BANKS(bg_ext_palette, bank_mask);
    switch (bank_mask) {
    case 0: DISPLAY_MAIN &= ~0x40000000; break;
    case 16: DISPLAY_MAIN |= 0x40000000; VRAM_E = 0x84; break;
    case 64: DISPLAY_MAIN |= 0x40000000; VRAM_G = 0x8C; break;
    case 96: VRAM_G = 0x8C;
    case 32: VRAM_F = 0x84; DISPLAY_MAIN |= 0x40000000; break;
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_02035e04(int bank_mask) {
    REASSIGN_BANKS(obj_ext_palette, bank_mask);
    switch (bank_mask) {
    case 32: DISPLAY_MAIN |= 0x80000000; VRAM_F = 0x85; break;
    case 64: DISPLAY_MAIN |= 0x80000000; VRAM_G = 0x85; break;
    case 0: DISPLAY_MAIN &= ~0x80000000; break;
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_02035c00(int bank_mask) {
    REASSIGN_BANKS(texture, bank_mask);
    if (bank_mask == 0) {
        *(vu16 *)0x04000060 &= 0xCFFE;
    } else {
        *(vu16 *)0x04000060 = (*(vu16 *)0x04000060 & ~0x3000) | 1;
        switch (bank_mask) {
        case 5: VRAM_A = 0x83; VRAM_C = 0x8B; break;
        case 9: VRAM_A = 0x83; VRAM_D = 0x8B; break;
        case 10: VRAM_B = 0x83; VRAM_D = 0x8B; break;
        case 11: VRAM_A = 0x83; VRAM_B = 0x8B; VRAM_D = 0x93; break;
        case 13: VRAM_A = 0x83; VRAM_C = 0x8B; VRAM_D = 0x93; break;
        case 8: VRAM_D = 0x83; break;
        case 12: VRAM_D = 0x8B;
        case 4: VRAM_C = 0x83; break;
        case 14: VRAM_D = 0x93;
        case 6: VRAM_C = 0x8B;
        case 2: VRAM_B = 0x83; break;
        case 15: VRAM_D = 0x9B;
        case 7: VRAM_C = 0x93;
        case 3: VRAM_B = 0x8B;
        case 1: VRAM_A = 0x83; break;
        }
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_02035b0c(int bank_mask) {
    REASSIGN_BANKS(texture_palette, bank_mask);
    switch (bank_mask) {
    case 64: VRAM_G = 0x83; break;
    case 96: VRAM_G = 0x8B;
    case 32: VRAM_F = 0x83; break;
    case 112: VRAM_G = 0x9B;
    case 48: VRAM_F = 0x93;
    case 16: VRAM_E = 0x83; break;
    case 0: break;
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_02035aec(int bank_mask) {
    data_02062c50.lcdc |= bank_mask;
    func_020363f4(bank_mask);
}

void func_02035a40(int bank_mask) {
    REASSIGN_BANKS(sub_bg, bank_mask);
    switch (bank_mask) {
    case 4: VRAM_C = 0x84; break;
    case 384: VRAM_I = 0x81;
    case 128: VRAM_H = 0x81; break;
    case 0: break;
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_020359c4(int bank_mask) {
    REASSIGN_BANKS(sub_obj, bank_mask);
    switch (bank_mask) {
    case 8: VRAM_D = 0x84; break;
    case 256: VRAM_I = 0x82; break;
    case 0: break;
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_02035938(int bank_mask) {
    REASSIGN_BANKS(sub_bg_ext_palette, bank_mask);
    switch (bank_mask) {
    case 128: DISPLAY_SUB |= 0x40000000; VRAM_H = 0x82; break;
    case 0: DISPLAY_SUB &= ~0x40000000; break;
    }
    func_020363f4(data_02062c50.lcdc);
}

void func_020358ac(int bank_mask) {
    REASSIGN_BANKS(sub_obj_ext_palette, bank_mask);
    switch (bank_mask) {
    case 256: DISPLAY_SUB |= 0x80000000; VRAM_I = 0x83; break;
    case 0: DISPLAY_SUB &= ~0x80000000; break;
    }
    func_020363f4(data_02062c50.lcdc);
}

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

extern u16 data_02062c48;
extern u16 data_02062c52;
extern u16 data_02062c54;
extern u16 data_02062c5c;
extern u16 data_02062c62;
extern u16 data_02062c64;

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

u32 func_02035588(void) {
    return data_02062c50.obj_ext_palette;
}

u32 func_02035578(void) {
    return data_02062c50.sub_obj_ext_palette;
}

/* Banks A-D are 128 KiB; E is 64 KiB; F/G/I are 16 KiB; H is 32 KiB. */
u32 func_02035524(u32 bank_mask) {
    u32 size = 0;
    if (bank_mask & 0x001) size += 0x20000;
    if (bank_mask & 0x002) size += 0x20000;
    if (bank_mask & 0x004) size += 0x20000;
    if (bank_mask & 0x008) size += 0x20000;
    if (bank_mask & 0x010) size += 0x10000;
    if (bank_mask & 0x020) size += 0x04000;
    if (bank_mask & 0x040) size += 0x04000;
    if (bank_mask & 0x080) size += 0x08000;
    if (bank_mask & 0x100) size += 0x04000;
    return size;
}

u32 func_0203550c(void) {
    return func_02035524(data_02062c50.obj);
}

u32 func_020354f4(void) {
    return func_02035524(data_02062c50.texture);
}

u32 func_020354dc(void) {
    return func_02035524(data_02062c50.texture_palette);
}

u32 func_020354c4(void) {
    return func_02035524(data_02062c50.sub_obj);
}

void func_02035444(void) {
    data_02062c50.lcdc = 0;
    data_02062c50.bg = 0;
    data_02062c50.obj = 0;
    data_02062c50.arm7 = 0;
    data_02062c50.texture = 0;
    data_02062c50.texture_palette = 0;
    data_02062c50.clear_image = 0;
    data_02062c50.bg_ext_palette = 0;
    data_02062c50.obj_ext_palette = 0;
    data_02062c50.sub_bg = 0;
    data_02062c50.sub_obj = 0;
    data_02062c50.sub_bg_ext_palette = 0;
    data_02062c50.sub_obj_ext_palette = 0;
    *(vu32 *)0x04000240 = 0;
    *(vu8 *)0x04000244 = 0;
    *(vu8 *)0x04000245 = 0;
    *(vu8 *)0x04000246 = 0;
    *(vu16 *)0x04000248 = 0;
}
