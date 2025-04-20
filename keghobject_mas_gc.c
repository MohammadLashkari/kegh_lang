#include "keghobject_mas_gc.h"
#include <stdlib.h>

void snek_object_free(kegh_object_t *obj) {
  switch (obj->kind) {
  case INTEGER:
  case FLOAT:
    break;

  case STRING:
    free(obj->data.v_string);
    break;

  case VECTOR3:
    break;

  case ARRAY:
    free(obj->data.v_array.elems);
    break;
  }
  free(obj);
}
