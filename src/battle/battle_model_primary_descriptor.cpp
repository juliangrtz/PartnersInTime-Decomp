extern "C" {
#include <game/model_resource.h>
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
extern u8 data_ov002_020c06a8;
extern const ModelResourceDescriptor data_ov002_020be758;
}
struct BattlePrimaryModelDescriptor {
    ModelResourceDescriptor base;
    u8 state[2];
    u16 padding;
    void *controller;
};
typedef char BattlePrimaryModelDescriptorSizeCheck[sizeof(BattlePrimaryModelDescriptor)==96?1:-1];
extern "C" BattlePrimaryModelDescriptor data_ov002_020c06ac;
extern "C" int BattleModel_InitPrimaryDescriptor(void) {
    int initialized = 0;
    if (!data_ov002_020c06a8) {
        if ((const void *)&data_ov002_020be758 < (void *)&data_ov002_020c06ac)
            func_0202cd2c(&data_ov002_020be758, &data_ov002_020c06ac, 88);
        else
            func_0202cc58(&data_ov002_020be758, &data_ov002_020c06ac, 88);
        data_ov002_020c06ac.base.resource_flags.screen = 0;
        data_ov002_020c06ac.base.flags.palette_allocation = 0;
        data_ov002_020c06ac.base.flags.unknown_16_18 = 2;
        data_ov002_020c06ac.base.resource_flags.unknown_00 = 0;
        data_ov002_020c06ac.state[0] = 0;
        data_ov002_020c06ac.state[1] = 0;
        data_ov002_020c06ac.controller = 0;
        data_ov002_020c06a8 = 1;
        initialized = 1;
    }
    return initialized;
}
