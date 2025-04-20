#pragma once
#include "keghobject_mas_gc.h"
#include "vm.h"

kegh_object_t *new_kegh_integer(vm_t *vm, int v);
kegh_object_t *new_kegh_float(vm_t *vm, float v);
kegh_object_t *new_kegh_string(vm_t *vm, char *v);
kegh_object_t *new_kegh_vector3(vm_t *vm, kegh_object_t *x, kegh_object_t *y,
                                kegh_object_t *z);
kegh_object_t *new_kegh_array(vm_t *vm, size_t size);
