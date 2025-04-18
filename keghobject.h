#include <stdbool.h>
#include <stddef.h>

typedef struct kegh_object kegh_object_t;

typedef enum kegh_object_kind {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY,

} kegh_object_kind_t;

typedef struct kegh_vector {
  kegh_object_t *x;
  kegh_object_t *y;
  kegh_object_t *z;

} kegh_vector_t;

typedef struct kegh_array {
  size_t size;
  kegh_object_t **elems;

} kegh_array_t;

typedef union kegh_object_data {
  int v_int;
  float v_float;
  char *v_string;
  kegh_vector_t v_vector3;
  kegh_array_t v_array;

} kegh_object_data_t;

typedef struct kegh_object {
  kegh_object_kind_t kind;
  kegh_object_data_t data;

} kegh_object_t;

kegh_object_t *new_kegh_integer(int v);
kegh_object_t *new_kegh_float(float v);
kegh_object_t *new_kegh_string(char *v);
kegh_object_t *new_kegh_vector3(kegh_object_t *x, kegh_object_t *y,
                                kegh_object_t *z);

kegh_object_t *new_kegh_array(size_t size);
bool kegh_array_set(kegh_object_t *kegh_obj, size_t index, kegh_object_t *v);
kegh_object_t *kegh_array_get(kegh_object_t *obj, size_t index);

int kegh_length(kegh_object_t *obj);
kegh_object_t *kegh_add(kegh_object_t *x, kegh_object_t *y);
