#include <nitro.h>

enum {
    REG_WIN0H = 0x04000040,
    REG_WIN1H = 0x04000042,
    REG_WIN0V = 0x04000044,
    REG_WIN1V = 0x04000046,
    REG_WININ = 0x04000048,
    REG_WINOUT = 0x0400004a,
    REG_SUB_WIN0H = 0x04001040,
    REG_SUB_WIN1H = 0x04001042,
    REG_SUB_WIN0V = 0x04001044,
    REG_SUB_WIN1V = 0x04001046,
    REG_SUB_WININ = 0x04001048,
    REG_SUB_WINOUT = 0x0400104a
};

enum { WINDOW_PLANE_MASK = 0x3f, WINDOW_EFFECT = 0x20 };

void func_ov005_020679fc(int screen, int window, u32 planes, int effects) {
    u32 value;
    if (screen == 0) {
        if (window == 0) {
            value = (*(vu16 *)REG_WININ & ~WINDOW_PLANE_MASK) | planes;
            if (effects) value |= WINDOW_EFFECT;
            *(vu16 *)REG_WININ = value;
        } else {
            value = (*(vu16 *)REG_WININ & ~(WINDOW_PLANE_MASK << 8)) | (planes << 8);
            if (effects) value |= WINDOW_EFFECT << 8;
            *(vu16 *)REG_WININ = value;
        }
    } else {
        if (window == 0) {
            value = (*(vu16 *)REG_SUB_WININ & ~WINDOW_PLANE_MASK) | planes;
            if (effects) value |= WINDOW_EFFECT;
            *(vu16 *)REG_SUB_WININ = value;
        } else {
            value = (*(vu16 *)REG_SUB_WININ & ~(WINDOW_PLANE_MASK << 8)) | (planes << 8);
            if (effects) value |= WINDOW_EFFECT << 8;
            *(vu16 *)REG_SUB_WININ = value;
        }
    }
}

void func_ov005_020679a4(int screen, u32 planes, int effects) {
    u32 value;
    if (screen == 0) {
        value = (*(vu16 *)REG_WINOUT & ~WINDOW_PLANE_MASK) | planes;
        if (effects) value |= WINDOW_EFFECT;
        *(vu16 *)REG_WINOUT = value;
    } else {
        value = (*(vu16 *)REG_SUB_WINOUT & ~WINDOW_PLANE_MASK) | planes;
        if (effects) value |= WINDOW_EFFECT;
        *(vu16 *)REG_SUB_WINOUT = value;
    }
}

void func_ov005_0206786c(int screen, int window, u32 left, u32 top, u32 right, u32 bottom) {
    if (screen == 0) {
        if (window == 0) {
            *(vu16 *)REG_WIN0H = ((left << 8) & 0xff00) | (right & 0xff);
            *(vu16 *)REG_WIN0V = ((top << 8) & 0xff00) | (bottom & 0xff);
        } else {
            *(vu16 *)REG_WIN1H = ((left << 8) & 0xff00) | (right & 0xff);
            *(vu16 *)REG_WIN1V = ((top << 8) & 0xff00) | (bottom & 0xff);
        }
    } else {
        if (window == 0) {
            *(vu16 *)REG_SUB_WIN0H = ((left << 8) & 0xff00) | (right & 0xff);
            *(vu16 *)REG_SUB_WIN0V = ((top << 8) & 0xff00) | (bottom & 0xff);
        } else {
            *(vu16 *)REG_SUB_WIN1H = ((left << 8) & 0xff00) | (right & 0xff);
            *(vu16 *)REG_SUB_WIN1V = ((top << 8) & 0xff00) | (bottom & 0xff);
        }
    }
}
