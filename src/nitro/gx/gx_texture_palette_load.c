#include <nitro.h>

extern int data_02059d78;
extern u8 *data_02062c8c;
extern int data_02062c90;
extern const u16 data_0204fac4[];
extern int func_02035804(void);
extern void func_02035b0c(int banks);
extern void func_0203b310(int channel);
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
