#include <nitro.h>

#define MAIN_DISPLAY (*(vu32 *)0x04000000)
#define SUB_DISPLAY (*(vu32 *)0x04001000)
#define MAIN_BG_CONTROL(index) (*(vu16 *)(0x04000008 + 2 * (index)))
#define SUB_BG_CONTROL(index) (*(vu16 *)(0x04001008 + 2 * (index)))
#define MAIN_BG_BASE 0x06000000
#define SUB_BG_BASE 0x06200000

/* Text-map addresses for the fixed text backgrounds. */
void *func_02036930(void) {
    int block = (MAIN_BG_CONTROL(0) & 0x1F00) >> 8;
    return (void *)(MAIN_BG_BASE +
                    (((MAIN_DISPLAY & 0x38000000) >> 27) << 16) +
                    (block << 11));
}

void *func_02036910(void) {
    u16 control = SUB_BG_CONTROL(0);
    return (void *)(SUB_BG_BASE + (((control & 0x1F00) >> 8) << 11));
}

void *func_020368dc(void) {
    int block = (MAIN_BG_CONTROL(1) & 0x1F00) >> 8;
    return (void *)(MAIN_BG_BASE +
                    (((MAIN_DISPLAY & 0x38000000) >> 27) << 16) +
                    (block << 11));
}

void *func_020368bc(void) {
    u16 control = SUB_BG_CONTROL(1);
    return (void *)(SUB_BG_BASE + (((control & 0x1F00) >> 8) << 11));
}

/* BG2/BG3 can select text maps, affine maps, or bitmap storage by mode. */
void *func_02036830(void) {
    int mode = MAIN_DISPLAY & 7;
    u32 control = MAIN_BG_CONTROL(2);
    u32 base = ((MAIN_DISPLAY & 0x38000000) >> 27) << 16;
    u32 screen = (control & 0x1F00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *)(MAIN_BG_BASE + base + (screen << 11));
    case 5:
        if (control & 0x80) return (void *)(MAIN_BG_BASE + (screen << 14));
        return (void *)(MAIN_BG_BASE + base + (screen << 11));
    case 6:
        return (void *)MAIN_BG_BASE;
    default:
        return 0;
    }
}

void *func_020367b0(void) {
    int mode = SUB_DISPLAY & 7;
    u32 control = SUB_BG_CONTROL(2);
    u32 screen = (control & 0x1F00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *)(SUB_BG_BASE + (screen << 11));
    case 5:
        if (control & 0x80) return (void *)(SUB_BG_BASE + (screen << 14));
        return (void *)(SUB_BG_BASE + (screen << 11));
    case 6:
        return 0;
    default:
        return 0;
    }
}

void *func_02036724(void) {
    int mode = MAIN_DISPLAY & 7;
    u32 control = MAIN_BG_CONTROL(3);
    u32 base = ((MAIN_DISPLAY & 0x38000000) >> 27) << 16;
    u32 screen = (control & 0x1F00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return (void *)(MAIN_BG_BASE + base + (screen << 11));
    case 3:
    case 4:
    case 5:
        if (control & 0x80) return (void *)(MAIN_BG_BASE + (screen << 14));
        return (void *)(MAIN_BG_BASE + base + (screen << 11));
    case 6:
        return 0;
    default:
        return 0;
    }
}

void *func_020366a4(void) {
    int mode = SUB_DISPLAY & 7;
    u32 control = SUB_BG_CONTROL(3);
    u32 screen = (control & 0x1F00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return (void *)(SUB_BG_BASE + (screen << 11));
    case 3:
    case 4:
    case 5:
        if (control & 0x80) return (void *)(SUB_BG_BASE + (screen << 14));
        return (void *)(SUB_BG_BASE + (screen << 11));
    case 6:
        return 0;
    default:
        return 0;
    }
}

/* Character-data addresses; bitmap modes have no separate tile data. */
void *func_02036670(void) {
    int block = (MAIN_BG_CONTROL(0) & 0x3C) >> 2;
    return (void *)(MAIN_BG_BASE +
                    (((MAIN_DISPLAY & 0x07000000) >> 24) << 16) +
                    (block << 14));
}

void *func_02036650(void) {
    u16 control = SUB_BG_CONTROL(0);
    return (void *)(SUB_BG_BASE + (((control & 0x3C) >> 2) << 14));
}

void *func_0203661c(void) {
    int block = (MAIN_BG_CONTROL(1) & 0x3C) >> 2;
    return (void *)(MAIN_BG_BASE +
                    (((MAIN_DISPLAY & 0x07000000) >> 24) << 16) +
                    (block << 14));
}

void *func_020365fc(void) {
    u16 control = SUB_BG_CONTROL(1);
    return (void *)(SUB_BG_BASE + (((control & 0x3C) >> 2) << 14));
}

void *func_020365a4(void) {
    int mode = MAIN_DISPLAY & 7;
    u32 control = MAIN_BG_CONTROL(2);
    if (mode < 5 || !(control & 0x80)) {
        u32 base = ((MAIN_DISPLAY & 0x07000000) >> 24) << 16;
        return (void *)(MAIN_BG_BASE + base + (((control & 0x3C) >> 2) << 14));
    }
    return 0;
}

void *func_0203655c(void) {
    int mode = SUB_DISPLAY & 7;
    u32 control = SUB_BG_CONTROL(2);
    if (mode < 5 || !(control & 0x80)) {
        return (void *)(SUB_BG_BASE + (((control & 0x3C) >> 2) << 14));
    }
    return 0;
}

void *func_020364fc(void) {
    int mode = MAIN_DISPLAY & 7;
    u32 control = MAIN_BG_CONTROL(3);
    if (mode < 3 || (mode < 6 && !(control & 0x80))) {
        u32 base = ((MAIN_DISPLAY & 0x07000000) >> 24) << 16;
        return (void *)(MAIN_BG_BASE + base + (((control & 0x3C) >> 2) << 14));
    }
    return 0;
}

void *func_020364ac(void) {
    int mode = SUB_DISPLAY & 7;
    u32 control = SUB_BG_CONTROL(3);
    if (mode < 3 || (mode < 6 && !(control & 0x80))) {
        return (void *)(SUB_BG_BASE + (((control & 0x3C) >> 2) << 14));
    }
    return 0;
}
