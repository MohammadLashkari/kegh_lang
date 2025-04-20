#pragma once
#include "keghobject_mas_gc.h"
#include "stack.h"

typedef struct VirtualMachine {
  stack_t *frames;
  stack_t *objects;
} vm_t;

typedef struct StackFrame {
  stack_t *references;
} frame_t;

vm_t *vm_new();
void vm_free(vm_t *vm);
void vm_frame_push(vm_t *vm, frame_t *frame);
void vm_track_object(vm_t *vm, kegh_object_t *obj);

frame_t *vm_new_frame(vm_t *vm);
void frame_free(frame_t *frame);
