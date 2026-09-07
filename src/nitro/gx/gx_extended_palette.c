#include <nitro.h>

extern int data_02059d78;
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
extern void func_0203b7b4(const void *source, void *destination, u32 size);

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
