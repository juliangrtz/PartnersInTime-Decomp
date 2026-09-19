/*
 * 2D VRAM uploads (ARM9 resident, 0x020379AC-0x0203869C).
 *
 * Uploading background and object tiles, screens and palettes for both engines.
 * The extended-palette paths need their bank mapped for CPU access first, which
 * is why they come as Begin/Load/End triples.
 */

#include <nitro.h>

extern int data_02059d78;
extern void func_0203b578(int channel, const void *source, void *destination, u32 size);
extern void func_0203b500(int channel, const void *source, void *destination, u32 size);
extern void func_0203b7b4(const void *source, void *destination, u32 size);
extern void MIi_CpuCopy16(const void *source, void *destination, u32 size);
extern void *func_020364ac(void);
extern void *func_020364fc(void);
extern void *func_0203655c(void);
extern void *func_020365a4(void);
extern void *func_020365fc(void);
extern void *func_0203661c(void);
extern void *func_02036650(void);
extern void *func_02036670(void);
extern void *func_020366a4(void);
extern void *func_02036724(void);
extern void *func_020367b0(void);
extern void *func_02036830(void);
extern void *func_020368bc(void);
extern void *func_020368dc(void);
extern void *func_02036910(void);
extern void *func_02036930(void);

/* Small transfers use CPU copies; larger transfers use the configured DMA
 * channel. Character/OAM data is word-sized, maps and palettes halfword-sized. */

void func_02038640(const void *source, u32 offset, u32 size) {
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, (u8 *)0x05000000 + offset, size);
    } else {
        MIi_CpuCopy16(source, (u8 *)0x05000000 + offset, size);
    }
}

void func_020385d8(const void *source, u32 offset, u32 size) {
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, (u8 *)0x05000400 + offset, size);
    } else {
        MIi_CpuCopy16(source, (u8 *)0x05000400 + offset, size);
    }
}

static inline void *MainObjectBase(void) { return (void *)0x06400000; }

void func_02038578(const void *source, u32 offset, u32 size) {
    u8 *base = MainObjectBase();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

static inline void *SubObjectBase(void) { return (void *)0x06600000; }

void func_02038518(const void *source, u32 offset, u32 size) {
    u8 *base = SubObjectBase();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_020384b0(const void *source, u32 offset, u32 size) {
    u8 *base = func_02036930();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_02038448(const void *source, u32 offset, u32 size) {
    u8 *base = func_02036910();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_020383e0(const void *source, u32 offset, u32 size) {
    u8 *base = func_020368dc();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_02038378(const void *source, u32 offset, u32 size) {
    u8 *base = func_020368bc();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_02038310(const void *source, u32 offset, u32 size) {
    u8 *base = func_02036830();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_020382a8(const void *source, u32 offset, u32 size) {
    u8 *base = func_020367b0();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_02038240(const void *source, u32 offset, u32 size) {
    u8 *base = func_02036724();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_020381d8(const void *source, u32 offset, u32 size) {
    u8 *base = func_020366a4();
    if (data_02059d78 != -1 && size > 28) {
        func_0203b500(data_02059d78, source, base + offset, size);
    } else {
        MIi_CpuCopy16(source, base + offset, size);
    }
}

void func_02038170(const void *source, u32 offset, u32 size) {
    u8 *base = func_02036670();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_02038108(const void *source, u32 offset, u32 size) {
    u8 *base = func_02036650();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_020380a0(const void *source, u32 offset, u32 size) {
    u8 *base = func_0203661c();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_02038038(const void *source, u32 offset, u32 size) {
    u8 *base = func_020365fc();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_02037fd0(const void *source, u32 offset, u32 size) {
    u8 *base = func_020365a4();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_02037f68(const void *source, u32 offset, u32 size) {
    u8 *base = func_0203655c();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_02037f00(const void *source, u32 offset, u32 size) {
    u8 *base = func_020364fc();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

void func_02037e98(const void *source, u32 offset, u32 size) {
    u8 *base = func_020364ac();
    if (data_02059d78 != -1 && size > 48) {
        func_0203b578(data_02059d78, source, base + offset, size);
    } else {
        func_0203b7b4(source, base + offset, size);
    }
}

extern int data_02062c6c;
extern u8 *data_02062c70;
extern int data_02062c74;
extern u32 data_02062c78;
extern u8 *data_02062c7c;
extern int data_02062c80;
extern int data_02062c84;
extern int func_020357b4(void);
extern int func_020357dc(void);
extern int func_0203582c(void);
extern int func_02035850(void);
extern void func_020358ac(int mask);
extern void func_02035938(int mask);
extern void func_02035e04(int mask);
extern void func_02035ebc(int mask);
extern void func_0203b310(int channel);
extern void func_0203b380(int channel, const void *source, void *destination,
                          u32 size, void *callback, void *argument);

/* Temporarily map extended palettes into LCDC, then restore the saved masks
 * after the final DMA transfer has completed. */
void GX_BeginLoadBGExtPltt(void) {
    int banks = func_02035850();
    data_02062c80 = banks;
    switch (banks) {
    case 0:
        break;
    case 16:
        data_02062c7c = (u8 *)0x06880000;
        data_02062c78 = 0;
        break;
    case 64:
        data_02062c7c = (u8 *)0x06894000;
        data_02062c78 = 0x4000;
        break;
    case 32:
    case 96:
        data_02062c7c = (u8 *)0x06890000;
        data_02062c78 = 0;
        break;
    }
}

void GX_LoadBGExtPltt(const void *source, u32 offset, u32 size) {
    u8 *destination = data_02062c7c + offset - data_02062c78;
    if (data_02059d78 != -1) {
        func_0203b380(data_02059d78, source, destination, size, 0, 0);
    } else {
        func_0203b7b4(source, destination, size);
    }
}

void GX_EndLoadBGExtPltt(void) {
    if (data_02059d78 != -1) func_0203b310(data_02059d78);
    func_02035ebc(data_02062c80);
    data_02062c80 = 0;
    data_02062c7c = 0;
    data_02062c78 = 0;
}

void GX_BeginLoadOBJExtPltt(void) {
    int banks = func_0203582c();
    data_02062c74 = banks;
    switch (banks) {
    case 0:
        break;
    case 32:
        data_02062c70 = (u8 *)0x06890000;
        break;
    case 64:
        data_02062c70 = (u8 *)0x06894000;
        break;
    }
}

void GX_LoadOBJExtPltt(const void *source, u32 offset, u32 size) {
    u8 *destination = data_02062c70 + offset;
    if (data_02059d78 != -1) {
        func_0203b380(data_02059d78, source, destination, size, 0, 0);
    } else {
        func_0203b7b4(source, destination, size);
    }
}

void GX_EndLoadOBJExtPltt(void) {
    if (data_02059d78 != -1) func_0203b310(data_02059d78);
    func_02035e04(data_02062c74);
    data_02062c74 = 0;
    data_02062c70 = 0;
}

void GXS_BeginLoadBGExtPltt(void) {
    data_02062c6c = func_020357dc();
}

void GXS_LoadBGExtPltt(const void *source, u32 offset, u32 size) {
    if (data_02059d78 != -1) {
        func_0203b380(data_02059d78, source, (u8 *)0x06898000 + offset,
                       size, 0, 0);
    } else {
        func_0203b7b4(source, (u8 *)0x06898000 + offset, size);
    }
}

void GXS_EndLoadBGExtPltt(void) {
    if (data_02059d78 != -1) func_0203b310(data_02059d78);
    func_02035938(data_02062c6c);
    data_02062c6c = 0;
}

void GXS_BeginLoadOBJExtPltt(void) {
    data_02062c84 = func_020357b4();
}

void GXS_LoadOBJExtPltt(const void *source, u32 offset, u32 size) {
    if (data_02059d78 != -1) {
        func_0203b380(data_02059d78, source, (u8 *)0x068A0000 + offset,
                       size, 0, 0);
    } else {
        func_0203b7b4(source, (u8 *)0x068A0000 + offset, size);
    }
}

void GXS_EndLoadOBJExtPltt(void) {
    if (data_02059d78 != -1) func_0203b310(data_02059d78);
    func_020358ac(data_02062c84);
    data_02062c84 = 0;
}
