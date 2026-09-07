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
