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
