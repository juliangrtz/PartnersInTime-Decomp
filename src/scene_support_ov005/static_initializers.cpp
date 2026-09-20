/* Scene pool and registry initialization, overlay 5, 0x02069D6C-0x02069EB8. */
#include <game/overlay005_pools.h>
#include <msl/object_register.h>
extern "C" {
extern Overlay5ElementPool data_ov005_0206a240;
extern Overlay5SpritePool data_ov005_0206a3d8;
extern Overlay5DrawPool data_ov005_0206a3f8;
extern Overlay5ItemPool data_ov005_0206aa18;
extern MslDestructorNode data_ov005_0206a234, data_ov005_0206a3cc,
    data_ov005_0206a3ec, data_ov005_0206aa0c, data_ov005_0206aa34;
struct Overlay5EntryList;
extern Overlay5EntryList data_ov005_0206aa40;
Overlay5ElementPool *func_ov005_02066864(Overlay5ElementPool *);
Overlay5SpritePool *func_ov005_02068ec8(Overlay5SpritePool *);
Overlay5DrawPool *func_ov005_020692a0(Overlay5DrawPool *);
Overlay5ItemPool *func_ov005_020698b0(Overlay5ItemPool *);
void func_ov005_02069b7c();
void func_ov005_02069ba0(void *);
void func_ov005_02067638(void *);
void func_020489e0(void *, int, u32, void (*)(void *), void (*)(void *));

/* Overlay loading calls these through the original .ctor table. Keep the
 * functions in .init; ordinary .text placement would change overlay ownership.
 * The runtime's destructor ABI supplies a mode argument and ignores the return
 * value; these existing cleanup functions use only their object argument.
 */
#pragma define_section SceneInit ".init" abs32 RX
#pragma section SceneInit begin
/* Metrowerks emits these .init functions in reverse source order. */
void func_ov005_02069eb4() {}
void Overlay5Registry_InitializeStatic()
{
    /* Two embedded registry markers, each with the native 28-byte stride. */
    func_020489e0(&data_ov005_0206aa40, 2, 28, func_ov005_02069ba0, func_ov005_02067638);
    MSL_RegisterGlobalObject(0, (MslGlobalDestructor)func_ov005_02069b7c, &data_ov005_0206aa34);
}
void Overlay5ItemPool_InitializeStatic()
{
    data_ov005_0206aa18.items = 0;
    data_ov005_0206aa18.links = 0;
    data_ov005_0206aa18.free_list = 0;
    data_ov005_0206aa18.taken_list = 0;
    data_ov005_0206aa18.count = 0;
    MSL_RegisterGlobalObject(&data_ov005_0206aa18, (MslGlobalDestructor)func_ov005_020698b0, &data_ov005_0206aa0c);
}
void Overlay5DrawPool_InitializeStatic()
{
    data_ov005_0206a3f8.entries = 0;
    data_ov005_0206a3f8.links = 0;
    data_ov005_0206a3f8.free_list = 0;
    data_ov005_0206a3f8.taken_list = 0;
    data_ov005_0206a3f8.count = 0;
    MSL_RegisterGlobalObject(&data_ov005_0206a3f8, (MslGlobalDestructor)func_ov005_020692a0, &data_ov005_0206a3ec);
}
void Overlay5SpritePool_InitializeStatic()
{
    data_ov005_0206a3d8.sprites = 0;
    data_ov005_0206a3d8.nodes = 0;
    data_ov005_0206a3d8.free_list = 0;
    data_ov005_0206a3d8.taken_list = 0;
    data_ov005_0206a3d8.count = 0;
    MSL_RegisterGlobalObject(&data_ov005_0206a3d8, (MslGlobalDestructor)func_ov005_02068ec8, &data_ov005_0206a3cc);
}
void func_ov005_02069da8() {}
void Overlay5ElementPool_InitializeStatic()
{
    data_ov005_0206a240.elements = 0;
    data_ov005_0206a240.links = 0;
    data_ov005_0206a240.free_list = 0;
    data_ov005_0206a240.taken_list = 0;
    data_ov005_0206a240.count = 0;
    MSL_RegisterGlobalObject(&data_ov005_0206a240, (MslGlobalDestructor)func_ov005_02066864, &data_ov005_0206a234);
}
#pragma section SceneInit end
}
