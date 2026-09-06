#include <nitro/gx_vram.h>

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
