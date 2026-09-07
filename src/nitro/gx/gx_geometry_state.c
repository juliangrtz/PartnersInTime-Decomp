#include <nitro.h>

#define REG16(address) (*(vu16 *)(address))
#define REG32(address) (*(vu32 *)(address))
#define GX_STATUS REG32(0x04000600)
#define DISPLAY_3D REG16(0x04000060)
#define MATRIX_MODE REG32(0x04000440)
#define MATRIX_POP REG32(0x04000448)
#define MATRIX_IDENTITY REG32(0x04000454)

extern int data_02059d78;
extern void func_02036cdc(void *fifo);
extern void func_0203b428(int channel, void *destination, u32 value, u32 size,
                          void *callback, void *argument);
extern void func_0203b5f4(int channel, void *destination, u32 value, u32 size);
extern void func_0203b7a0(u32 value, void *destination, u32 size);
extern void MI_Copy32B(const void *source, void *destination);
void func_02036e28(void);
void func_02036f78(void);
void func_02037024(void);
void func_020370d4(void);
int func_02036dfc(int *level);
int func_02036dd0(int *level);

void func_020372ec(int sine, int cosine) {
    REG32(0x04000468) = cosine;
    REG32(0x04000468) = sine;
    REG32(0x04000468) = 0;
    REG32(0x04000468) = -sine;
    REG32(0x04000468) = cosine;
    REG32(0x04000468) = 0;
    REG32(0x04000468) = 0;
    REG32(0x04000468) = 0;
    REG32(0x04000468) = 0x1000;
}

void func_02037190(void) {
    func_020370d4();
    REG32(0x04000504) = 0;
    while (GX_STATUS & 0x08000000) {}
    DISPLAY_3D = 0;
    GX_STATUS = 0;
    REG32(0x04000010) = 0;
    DISPLAY_3D |= 0x2000;
    DISPLAY_3D |= 0x1000;
    DISPLAY_3D &= ~0x3002;
    DISPLAY_3D = (DISPLAY_3D & ~0x3000) | 0x10;
    DISPLAY_3D &= 0xCFFB;
    GX_STATUS |= 0x8000;
    GX_STATUS = (GX_STATUS & ~0xC0000000) | 0x80000000;
    func_02037024();
    REG32(0x04000350) = 0;
    REG16(0x04000354) = 0x7FFF;
    REG16(0x04000356) = 0;
    REG32(0x04000358) = 0;
    REG16(0x0400035C) = 0;
    REG16(0x04000008) &= ~3;
    func_02036e28();
    REG32(0x040004A4) = 0x1F0080;
    REG32(0x040004A8) = 0;
    REG32(0x040004AC) = 0;
}

void func_02037108(void) {
    while (GX_STATUS & 0x08000000) {}
    GX_STATUS |= 0x8000;
    DISPLAY_3D |= 0x2000;
    DISPLAY_3D |= 0x1000;
    func_02036f78();
    REG32(0x040004A4) = 0x1F0080;
    REG32(0x040004A8) = 0;
    REG32(0x040004AC) = 0;
}

void func_020370d4(void) {
    func_02036cdc((void *)0x04000400);
    while (GX_STATUS & 0x08000000) {}
}

void func_02037024(void) {
    int position_level, projection_level;
    GX_STATUS |= 0x8000;
    while (func_02036dfc(&position_level)) {}
    while (func_02036dd0(&projection_level)) {}
    MATRIX_MODE = 3;
    MATRIX_IDENTITY = 0;
    MATRIX_MODE = 0;
    if (projection_level) MATRIX_POP = projection_level;
    MATRIX_IDENTITY = 0;
    MATRIX_MODE = 2;
    MATRIX_POP = position_level;
    MATRIX_IDENTITY = 0;
}

void func_02036f78(void) {
    int position_level, projection_level;
    GX_STATUS |= 0x8000;
    while (func_02036dfc(&position_level)) {}
    while (func_02036dd0(&projection_level)) {}
    MATRIX_MODE = 3;
    MATRIX_IDENTITY = 0;
    MATRIX_MODE = 0;
    if (projection_level) MATRIX_POP = projection_level;
    MATRIX_MODE = 2;
    MATRIX_POP = position_level;
    MATRIX_IDENTITY = 0;
}

void func_02036f24(int enable, int mode, int slope, int offset) {
    if (enable) {
        u32 setting;
        REG16(0x0400035C) = offset;
        setting = (slope << 8) | (mode << 6) | 0x80;
        DISPLAY_3D = (DISPLAY_3D & ~0x3F40) | setting;
    } else {
        DISPLAY_3D &= 0xCF7F;
    }
}

void func_02036f10(const void *table) {
    MI_Copy32B(table, (void *)0x04000360);
}

void func_02036ee0(int color, int alpha, int depth, int polygon_id, int fog) {
    u32 value = color | (alpha << 16) | (polygon_id << 24);
    if (fog) value |= 0x8000;
    REG32(0x04000350) = value;
    REG16(0x04000354) = depth;
}

void func_02036e28(void) {
    int i;
    if (data_02059d78 != -1) {
        func_0203b428(data_02059d78, (void *)0x04000330, 0, 16, 0, 0);
        func_0203b5f4(data_02059d78, (void *)0x04000360, 0, 96);
    } else {
        volatile u32 edge_color = 0;
        func_0203b7a0(edge_color, (void *)0x04000330, 16);
        {
            volatile u32 fog_table = 0;
            func_0203b7a0(fog_table, (void *)0x04000360, 96);
        }
    }
    for (i = 0; i < 32; i++) REG32(0x040004D0) = 0;
}

int func_02036dfc(int *level) {
    if (GX_STATUS & 0x4000) return -1;
    *level = (GX_STATUS & 0x1F00) >> 8;
    return 0;
}

int func_02036dd0(int *level) {
    if (GX_STATUS & 0x4000) return -1;
    *level = (GX_STATUS & 0x2000) >> 13;
    return 0;
}

int func_02036d80(u16 *vector) {
    if (GX_STATUS & 1) return -1;
    vector[0] = REG16(0x04000630);
    vector[1] = REG16(0x04000632);
    vector[2] = REG16(0x04000634);
    return 0;
}

void func_02036d70(u32 offset) {
    REG32(0x04000010) = offset;
}
