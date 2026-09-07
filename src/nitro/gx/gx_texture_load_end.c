#include <nitro.h>

extern int data_02059d78;
extern u8 *data_02062c88;
extern int data_02062c94;
extern u32 data_02062c98;
extern u32 data_02062c9c;
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

