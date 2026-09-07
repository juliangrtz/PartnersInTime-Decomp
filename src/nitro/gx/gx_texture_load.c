#include <nitro.h>
extern u8 *data_02062c88;
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
