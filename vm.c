#include "vm.h"
#include "keghobject_mas_gc.h"
#include "stack.h"
#include <stdlib.h>

const int INIT_FRAMES = 8;
const int INIT_OBJECTS = 8;
const int INIT_REFRENCES = 8;

vm_t *vm_new() {
  vm_t *vm = malloc(sizeof(vm_t));
  if (vm == NULL) {
    return NULL;
  }
  vm->frames = stack_new(INIT_FRAMES);
  if (vm->frames == NULL) {
    free(vm);
    return NULL;
  }
  vm->objects = stack_new(INIT_OBJECTS);
  if (vm->objects == NULL) {
    stack_free(vm->frames);
    free(vm);
    return NULL;
  }

  return vm;
}

void vm_free(vm_t *vm) {
  if (vm == NULL) {
    return;
  }
  for (size_t i = 0; i < vm->frames->count; i++) {
    frame_free(vm->frames->data[i]);
  }
  stack_free(vm->frames);

  for (size_t i = 0; i < vm->objects->count; i++) {
    kegh_object_free(vm->objects->data[i]);
  }
  stack_free(vm->objects);

  free(vm);
}

void vm_frame_push(vm_t *vm, frame_t *frame) {
  if (vm == NULL || frame == NULL) {
    return;
  }
  stack_push(vm->frames, (void *)frame);
}

frame_t *vm_new_frame(vm_t *vm) {
  frame_t *frame = malloc(sizeof(frame_t));
  if (frame == NULL) {
    return NULL;
  }
  frame->references = stack_new(INIT_REFRENCES);
  if (frame->references == NULL) {
    free(frame);
    return NULL;
  }
  vm_frame_push(vm, frame);

  return frame;
}

void vm_track_object(vm_t *vm, kegh_object_t *obj) {
  if (vm->objects == NULL || obj == NULL) {
    return;
  }
  stack_push(vm->objects, (void *)obj);
}

void frame_reference_object(frame_t *frame, kegh_object_t *obj) {
  if (frame == NULL || obj == NULL) {
    return;
  }
  stack_push(frame->references, (void *)obj);
}

void frame_free(frame_t *frame) {
  if (frame == NULL) {
    return;
  }
  stack_free(frame->references);
  free(frame);
}
