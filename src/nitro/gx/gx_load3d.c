#include <nitro.h>

extern u8 *data_02062c88;
extern int data_02062c94;
extern u32 data_02062c98;
extern u32 data_02062c9c;
extern const u16 data_0204fad4[][3];
extern const u16 data_0204fad6[][3];
extern const u16 data_0204fad8[][3];
extern int func_02035818(void);

/* The texture mapping table describes two physical spans and the logical
 * offset where a transfer must switch to the second span. */
void func_02038984(void) {
    int banks = func_02035818();
    u32 first = data_0204fad4[banks][0] << 12;
    u32 second = data_0204fad6[banks][0] << 12;
    u32 split = data_0204fad8[banks][0] << 12;
    data_02062c94 = banks;
    data_02062c88 = (u8 *)first;
    data_02062c98 = second;
    data_02062c9c = split;
}

extern u32 data_02062c98, data_02062c9c;
extern int data_02059d78;
extern void func_0203b380(int, const void *, void *, u32, void *, void *);
extern void func_0203b578(int, const void *, void *, u32);
extern void func_0203b7b4(const void *, void *, u32);

void GX_LoadTex(const u8 *source, u32 offset, u32 size)
{
    u8 *second = (u8 *)data_02062c98;
    u8 *destination;
    if (!second) {
        destination = data_02062c88 + offset;
    } else if (offset + size < data_02062c9c) {
        destination = data_02062c88 + offset;
    } else if (offset >= data_02062c9c) {
        destination = second + offset - data_02062c9c;
    } else {
        u32 first_size = data_02062c9c - offset;
        destination = data_02062c88 + offset;
        if (data_02059d78 != -1 && first_size > 48)
            func_0203b578(data_02059d78, source, destination, first_size);
        else
            func_0203b7b4(source, destination, first_size);
        if (data_02059d78 != -1)
            func_0203b380(data_02059d78, source + first_size, second, size - first_size, 0, 0);
        else
            func_0203b7b4(source + first_size, second, size - first_size);
        return;
    }
    if (data_02059d78 != -1)
        func_0203b380(data_02059d78, source, destination, size, 0, 0);
    else
        func_0203b7b4(source, destination, size);
}

extern void func_02035c00(int banks);
extern void func_0203b310(int channel);

void func_020387b0(void) {
    if (data_02059d78 != -1) func_0203b310(data_02059d78);
    func_02035c00(data_02062c94);
    data_02062c9c = 0;
    data_02062c98 = 0;
    data_02062c88 = 0;
    data_02062c94 = 0;
}

extern u8 *data_02062c8c;
extern int data_02062c90;
extern const u16 data_0204fac4[];
extern int func_02035804(void);
extern void func_02035b0c(int banks);
extern void func_0203b380(int channel, const void *source, void *destination,
                          u32 size, void *callback, void *argument);
extern void func_0203b7b4(const void *source, void *destination, u32 size);

void func_02038768(void) {
    int banks = func_02035804();
    u32 base = data_0204fac4[banks >> 4];
    data_02062c90 = banks;
    data_02062c8c = (u8 *)(base << 12);
}

void func_020386f4(const void *source, u32 offset, u32 size) {
    u8 *destination = data_02062c8c + offset;
    if (data_02059d78 != -1) {
        func_0203b380(data_02059d78, source, destination, size, 0, 0);
    } else {
        func_0203b7b4(source, destination, size);
    }
}

void func_0203869c(void) {
    if (data_02059d78 != -1) func_0203b310(data_02059d78);
    func_02035b0c(data_02062c90);
    data_02062c90 = 0;
    data_02062c8c = 0;
}
