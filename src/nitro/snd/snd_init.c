#include <nitro/os_sync.h>
#include <nitro/snd_command.h>

extern int data_02063040;
extern OsMutex data_02063044;
extern void func_02039e5c(OsMutex *mutex);
extern void func_02039ecc(OsMutex *mutex);
extern void func_02039f58(OsMutex *mutex);

void func_0203c2c0(void) {
    if (data_02063040) return;
    data_02063040 = 1;
    func_02039f58(&data_02063044);
    func_0203ca18();
    func_0203cbe0();
}
void func_0203c2ac(void) { func_02039ecc(&data_02063044); }
void func_0203c298(void) { func_02039e5c(&data_02063044); }
