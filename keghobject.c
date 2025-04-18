#include "keghobject.h"
#include <stdlib.h>
#include <string.h>

kegh_object_t *new_kegh_integer(int v) {
  kegh_object_t *obj = malloc(sizeof(kegh_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->kind = INTEGER;
  obj->data.v_int = v;

  return obj;
}

kegh_object_t *new_kegh_float(float v) {
  kegh_object_t *obj = malloc(sizeof(kegh_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->kind = FLOAT;
  obj->data.v_float = v;

  return obj;
}

kegh_object_t *new_kegh_string(char *v) {
  kegh_object_t *obj = malloc(sizeof(kegh_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->kind = STRING;
  obj->data.v_string = malloc(strlen(v) + 1);
  if (obj->data.v_string == NULL) {
    return NULL;
  }
  strcpy(obj->data.v_string, v);

  return obj;
}

kegh_object_t *new_kegh_vector3(kegh_object_t *x, kegh_object_t *y,
                                kegh_object_t *z) {

  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  kegh_object_t *obj = malloc(sizeof(kegh_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->kind = STRING;
  obj->data.v_vector3.x = x;
  obj->data.v_vector3.y = y;
  obj->data.v_vector3.z = z;

  return obj;
}

kegh_object_t *new_kegh_array(size_t size) {
  kegh_object_t *obj = malloc(sizeof(kegh_object_t));
  if (obj == NULL) {
    return NULL;
  }

  kegh_object_t **elems = calloc(size, sizeof(kegh_object_t *));
  if (obj == NULL) {
    free(obj);
    return NULL;
  }
  kegh_array_t arr = {
      .size = size,
      .elems = elems,
  };

  obj->kind = ARRAY;
  obj->data.v_array = arr;

  return obj;
}

bool kegh_array_set(kegh_object_t *obj, size_t index, kegh_object_t *v) {
  if (obj == NULL || v == NULL) {
    return false;
  }
  if (obj->kind != ARRAY) {
    return false;
  }
  if (obj->data.v_array.size <= index) {
    return false;
  }
  obj->data.v_array.elems[index] = v;

  return true;
}

kegh_object_t *kegh_array_get(kegh_object_t *obj, size_t index) {
  if (obj == NULL) {
    return NULL;
  }
  if (obj->kind != ARRAY) {
    return NULL;
  }
  if (obj->data.v_array.size <= index) {
    return NULL;
  }

  return obj->data.v_array.elems[index];
}

int kegh_length(kegh_object_t *obj) {
  if (obj == NULL) {
    return -1;
  }
  switch (obj->kind) {
  case INTEGER:
    return 1;
  case FLOAT:
    return 1;
  case STRING:
    return strlen(obj->data.v_string);
  case VECTOR3:
    return 3;
  case ARRAY:
    return obj->data.v_array.size;
  default:
    return -1;
  }
}

kegh_object_t *kegh_add(kegh_object_t *x, kegh_object_t *y) {
  if (x == NULL || y == NULL) {
    return NULL;
  }
  if (x->kind == INTEGER) {
    switch (y->kind) {
    case INTEGER:
      return new_kegh_integer(x->data.v_int + y->data.v_int);
    case FLOAT:
      return new_kegh_float((float)x->data.v_int + y->data.v_float);
    default:
      return NULL;
    }
  }

  if (x->kind == FLOAT) {
    switch (y->kind) {
    case FLOAT:
      return new_kegh_float(x->data.v_float + y->data.v_float);
    case INTEGER:
      return new_kegh_float(x->data.v_float + (float)y->data.v_int);
    default:
      return NULL;
    }
  }

  if (x->kind == STRING) {
    switch (y->kind) {
    case STRING: {
      size_t len = strlen(x->data.v_string) + strlen(y->data.v_string) + 1;
      char *tmp = calloc(len, sizeof(char));
      if (tmp == NULL) {
        return NULL;
      }
      strcat(tmp, x->data.v_string);
      strcat(tmp, y->data.v_string);
      kegh_object_t *new_str = new_kegh_string(tmp);
      free(tmp);
      return new_str;
    }
    default:
      return NULL;
    }
  }

  if (x->kind == VECTOR3) {
    switch (y->kind) {
    case VECTOR3:
      return new_kegh_vector3(
          kegh_add(x->data.v_vector3.x, y->data.v_vector3.x),
          kegh_add(x->data.v_vector3.y, y->data.v_vector3.y),
          kegh_add(x->data.v_vector3.z, y->data.v_vector3.z));
    default:
      return NULL;
    }
  }

  if (x->kind == ARRAY) {
    switch (y->kind) {
    case ARRAY: {
      size_t new_size = x->data.v_array.size + y->data.v_array.size;
      kegh_object_t *new_arr = new_kegh_array(new_size);

      for (size_t i = 0; i < x->data.v_array.size; i++) {
        kegh_array_set(new_arr, i, kegh_array_get(x, i));
      }
      for (size_t i = 0; i < y->data.v_array.size; i++) {
        kegh_array_set(new_arr, i + x->data.v_array.size, kegh_array_get(y, i));
      }
      return new_arr;
    }
    default:
      return NULL;
    }
  }

  return NULL;
}
