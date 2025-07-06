#ifndef VM_H
#define VM_H
#include "stack.h"
#include "object.h"

typedef struct VirtualMachine {
  stack_t *frames;
  stack_t *objects;
} vm_t;

typedef struct Frame {
  stack_t *references;
} frame_t;

/// MARK & SWEEP FUNC
void mark(vm_t *vm);
void trace(vm_t *vm);
void sweep(vm_t *vm);

/// VM FUNC
void vm_collect_garbage(vm_t *vm);
vm_t *vm_new(void);
void vm_free(vm_t *vm);
void vm_track_object(vm_t *vm, snake_obj_t *obj);
void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t *vm_new_frame(vm_t *vm);



// FRAME FUNC
void frame_free(frame_t *frame);
void frame_reference_object(frame_t *frame, snake_obj_t *obj);


snake_obj_t *new_snek_integer(vm_t *vm, int value);
snake_obj_t *new_snek_float(vm_t *vm, float value);
snake_obj_t *new_snek_string(vm_t *vm, char *value);
snake_obj_t *new_snek_vector(
  vm_t *vm, snake_obj_t *x, snake_obj_t *y, snake_obj_t *z
);
snake_obj_t *new_snek_array(vm_t *vm, size_t size);
void snek_object_free(snake_obj_t *obj);

#endif // VM_H
