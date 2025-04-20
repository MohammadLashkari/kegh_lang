#pragma once
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
  bool is_marked;

} kegh_object_t;

void kegh_object_free(kegh_object_t *obj);
