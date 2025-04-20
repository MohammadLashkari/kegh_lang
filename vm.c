#include "vm.h"
#include "keghobject_mas_gc.h"
#include "stack.h"
#include <stdlib.h>

const int INIT_SIZE = 8;

vm_t *vm_new() {
  vm_t *vm = malloc(sizeof(vm_t));
  if (vm == NULL) {
    return NULL;
  }
  vm->frames = stack_new(INIT_SIZE);
  if (vm->frames == NULL) {
    free(vm);
    return NULL;
  }
  vm->objects = stack_new(INIT_SIZE);
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
  frame->references = stack_new(INIT_SIZE);
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

void mark(vm_t *vm) {
  if (vm == NULL) {
    return;
  }

  for (size_t i = 0; i < vm->frames->count; i++) {
    frame_t *frame = vm->frames->data[i];
    for (size_t j = 0; j < frame->references->count; j++) {
      kegh_object_t *obj = frame->references->data[j];
      obj->is_marked = true;
    }
  }
}

void trace(vm_t *vm) {
  if (vm == NULL) {
    return;
  }
  stack_t *gray_objects = stack_new(INIT_SIZE);
  if (gray_objects == NULL) {
    return;
  }

  for (size_t i = 0; i < vm->objects->count; i++) {
    kegh_object_t *obj = vm->objects->data[i];
    if (obj->is_marked) {
      stack_push(gray_objects, (void *)obj);
    }
  }

  while (gray_objects->count > 0) {
    trace_blacken_object(gray_objects, stack_pop(gray_objects));
  }

  stack_free(gray_objects);
}

void trace_blacken_object(stack_t *gray_objects, kegh_object_t *ref) {
  if (ref == NULL) {
    return;
  }
  switch (ref->kind) {
  case INTEGER:
  case FLOAT:
  case STRING:
    break;
  case VECTOR3:
    trace_mark_object(gray_objects, ref->data.v_vector3.x);
    trace_mark_object(gray_objects, ref->data.v_vector3.y);
    trace_mark_object(gray_objects, ref->data.v_vector3.z);
    break;
  case ARRAY:
    for (size_t i = 0; i < ref->data.v_array.size; i++) {
      trace_mark_object(gray_objects, ref->data.v_array.elems[i]);
    }
    break;
  }
}

void trace_mark_object(stack_t *gray_objects, kegh_object_t *ref) {
  if (ref == NULL || ref->is_marked) {
    return;
  }
  ref->is_marked = true;
  stack_push(gray_objects, (void *)ref);
}

void sweep(vm_t *vm) {
  if (vm == NULL) {
    return;
  }
  for (size_t i = 0; i < vm->objects->count; i++) {
    kegh_object_t *obj = vm->objects->data[i];
    if (obj->is_marked) {
      obj->is_marked = false;
    } else {
      kegh_object_free(obj);
      vm->objects->data[i] = NULL;
    }
  }
  stack_remove_nulls(vm->objects);
}

void vm_collect_garbage(vm_t *vm) {
  if (vm == NULL) {
    return;
  }
  mark(vm);
  trace(vm);
  sweep(vm);
}
