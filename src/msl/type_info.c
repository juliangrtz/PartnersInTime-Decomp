/*
 * RTTI destruction for type_info, __class_type_info and __si_class_type_info.
 * Keep the base and complete destructor entry points distinct. Deleting
 * variants also release the allocation and return its former pointer, as
 * required by the native ABI; they never read the object after release.
 */
#include <nitro.h>
#include <game/heap.h>
typedef struct MslTypeInfo {
    const void *vtable;
    const char *name;
} MslTypeInfo;
/* Only the shared prefix is touched; single-inheritance records also have
 * a base-type pointer. Vtables remain in the original data section. */
typedef struct MslSingleInheritanceTypeInfo {
    MslTypeInfo type;
    const MslTypeInfo *base;
} MslSingleInheritanceTypeInfo;
typedef char MslTypeInfoSize[sizeof(MslTypeInfo) == 8 ? 1 : -1];
typedef char MslSingleInheritanceTypeInfoSize[
    sizeof(MslSingleInheritanceTypeInfo) == 12 ? 1 : -1];
extern const u32 data_02059ed4[], data_02059ee4[], data_02059ef4[];
MslTypeInfo *MSL_TypeInfo_DestroyBase(MslTypeInfo *object);
MslTypeInfo *MSL_ClassTypeInfo_DestroyBase(MslTypeInfo *object);

MslTypeInfo *MSL_TypeInfo_Destroy(MslTypeInfo *object)
{
    object->vtable = data_02059ed4;
    return object;
}

MslTypeInfo *MSL_TypeInfo_Delete(MslTypeInfo *object)
{
    object->vtable = data_02059ed4;
    GameHeap_Delete(object);
    return object;
}

MslTypeInfo *MSL_TypeInfo_DestroyBase(MslTypeInfo *object)
{
    object->vtable = data_02059ed4;
    return object;
}

MslTypeInfo *MSL_ClassTypeInfo_Destroy(MslTypeInfo *object)
{
    object->vtable = data_02059ef4;
    MSL_TypeInfo_DestroyBase(object);
    return object;
}

MslTypeInfo *MSL_ClassTypeInfo_Delete(MslTypeInfo *object)
{
    object->vtable = data_02059ef4;
    MSL_TypeInfo_DestroyBase(object);
    GameHeap_Delete(object);
    return object;
}

MslTypeInfo *MSL_ClassTypeInfo_DestroyBase(MslTypeInfo *object)
{
    object->vtable = data_02059ef4;
    MSL_TypeInfo_DestroyBase(object);
    return object;
}

MslTypeInfo *MSL_SingleInheritanceTypeInfo_Destroy(MslTypeInfo *object)
{
    object->vtable = data_02059ee4;
    MSL_ClassTypeInfo_DestroyBase(object);
    return object;
}

MslTypeInfo *MSL_SingleInheritanceTypeInfo_Delete(MslTypeInfo *object)
{
    object->vtable = data_02059ee4;
    MSL_ClassTypeInfo_DestroyBase(object);
    GameHeap_Delete(object);
    return object;
}
