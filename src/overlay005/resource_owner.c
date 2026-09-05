#include <game/overlay005_resource.h>

extern void *func_ov005_02068c80(void);
extern void func_ov005_02068c54(void *resource);
extern void *func_ov005_0206964c(void);
extern void func_ov005_020695cc(void *resource);
extern void *func_ov005_02069084(void *resource, int selector);

void Overlay5ResourceA_Release(Overlay5ResourceOwner *owner);
void Overlay5ResourceB_Release(Overlay5ResourceOwner *owner);

/* Metrowerks emits C functions in reverse source order. */
void Overlay5ResourceB_Release(Overlay5ResourceOwner *owner) {
    if (owner->resource != 0) {
        func_ov005_020695cc(owner->resource);
    }
    owner->resource = 0;
}

void *Overlay5ResourceB_Attach(Overlay5ResourceOwner *owner, void *resource,
                               int release_with_owner) {
    if (resource == 0) {
        resource = func_ov005_0206964c();
    }
    if (release_with_owner) {
        owner->release_callback = Overlay5ResourceB_Release;
    }
    owner->resource = resource;
    return resource;
}

void *Overlay5ResourceB_Get(Overlay5ResourceOwner *owner) {
    return owner->resource;
}

void Overlay5ResourceA_Release(Overlay5ResourceOwner *owner) {
    if (owner->resource != 0) {
        func_ov005_02068c54(owner->resource);
    }
    owner->resource = 0;
}

void *Overlay5ResourceA_Attach(Overlay5ResourceOwner *owner, void *resource,
                               int release_with_owner) {
    if (resource == 0) {
        resource = func_ov005_02068c80();
    }
    if (release_with_owner) {
        owner->release_callback = Overlay5ResourceA_Release;
    }
    owner->resource = resource;
    return resource;
}

void *Overlay5ResourceA_Get(Overlay5ResourceOwner *owner) {
    return owner->resource;
}

void Overlay5ResourceA_ReleaseCallback(Overlay5ResourceOwner *owner) {
    Overlay5ResourceA_Release(owner);
}

void *Overlay5ResourceB_ApplySelector(Overlay5ResourceOwner *owner) {
    void *resource = Overlay5ResourceB_Get(owner);
    return func_ov005_02069084(resource, owner->selector & 0xFF);
}

void *Overlay5ResourceA_ApplySelector(Overlay5ResourceOwner *owner) {
    void *resource = Overlay5ResourceA_Get(owner);
    return func_ov005_02069084(resource, owner->selector & 0xFF);
}
