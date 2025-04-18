#include "keghobject.h"
#include <stdio.h>

int main() {

  kegh_object_t *arr1 = new_kegh_array(10);
  kegh_object_t *arr2 = new_kegh_array(10);

  for (int i = 0; i < 10; i++) {
    kegh_object_t *v = new_kegh_integer(i);
    kegh_array_set(arr1, i, v);
    kegh_array_set(arr2, i, v);
  }

  kegh_object_t *arr3 = kegh_add(arr1, arr2);

  for (size_t i = 0; i < arr3->data.v_array.size; i++) {
    printf("%d ", kegh_array_get(arr3, i)->data.v_int);
  }
  printf("\n");

  return 0;
}
