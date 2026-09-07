#include <nitro/os_lock.h>

extern void func_0203a384(void);
extern void func_0203d3ac(void);
extern void func_0203a2f0(void);
extern void func_02038e20(void);
extern void func_0203a66c(void);
extern void func_0203bd48(void);
extern void func_0203ad78(void);
extern void func_0203b124(void);
extern void func_020396d8(void);
extern void func_0203af80(void);
extern void CTRDG_Init(void);
extern void CARD_Init(void);
extern void PM_Init(void);

void OS_Init(void)
{
    func_0203a384();
    func_0203d3ac();
    OS_InitLock();
    func_0203a2f0();
    func_02038e20();
    func_0203a66c();
    func_0203bd48();
    func_0203ad78();
    func_0203b124();
    func_020396d8();
    func_0203af80();
    CTRDG_Init();
    CARD_Init();
    PM_Init();
}
