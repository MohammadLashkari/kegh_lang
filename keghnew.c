#include "keghobject_mas_gc.h"
#include "vm.h"
#include <stdlib.h>
#include <string.h>

kegh_object_t *_new_kegh_object(vm_t *vm) {
  kegh_object_t *obj = calloc(1, sizeof(kegh_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->is_marked = false;
  vm_track_object(vm, obj);
  return obj;
}

kegh_object_t *new_kegh_integer(vm_t *vm, int v) {
  kegh_object_t *obj = _new_kegh_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = v;

  return obj;
}

kegh_object_t *new_kegh_float(vm_t *vm, float v) {
  kegh_object_t *obj = _new_kegh_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = FLOAT;
  obj->data.v_float = v;
  return obj;
}

kegh_object_t *new_kegh_string(vm_t *vm, char *v) {
  kegh_object_t *obj = _new_kegh_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = STRING;
  obj->data.v_string = malloc(strlen(v) + 1);
  if (obj->data.v_string == NULL) {
    free(obj);
    return NULL;
  }

  strcpy(obj->data.v_string, v);

  return obj;
}

kegh_object_t *new_kegh_array(vm_t *vm, size_t size) {
  kegh_object_t *obj = _new_kegh_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  kegh_object_t **elems = calloc(size, sizeof(kegh_object_t *));
  if (elems == NULL) {
    free(obj);
    return NULL;
  }

  obj->kind = ARRAY;
  obj->data.v_array = (kegh_array_t){.size = size, .elems = elems};

  return obj;
}

kegh_object_t *new_kegh_vector3(vm_t *vm, kegh_object_t *x, kegh_object_t *y,
                                kegh_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  kegh_object_t *obj = _new_kegh_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = VECTOR3;
  obj->data.v_vector3 = (kegh_vector_t){.x = x, .y = y, .z = z};

  return obj;
}
