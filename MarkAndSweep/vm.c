#include "vm.h"
#include "object.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MARK & SWEEP FUNC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// MAIN GARBAGE FUNC
void vm_collect_garbage(vm_t *vm) {
  if(!vm) return;

  mark(vm);
  trace(vm);
  sweep(vm);

}


void sweep(vm_t *vm){
  if(!vm) return;

  for(int i = 0; i < (int)vm->objects->count; ++i){
    snake_obj_t *obj = vm->objects->data[i];
    if(obj->is_marked) obj->is_marked = 0;
    snek_object_free(obj);
    vm->objects->data[i] = NULL;
  }

  stack_remove_nulls(vm->objects);
}


void mark(vm_t *vm){
  if (!vm) return;

  for (int i = 0; i < (int)vm->frames->count; ++i){
    frame_t *frame = vm->frames->data[i];

    for (int j = 0; j < (int)frame->references->count; ++j){
      snake_obj_t *obj = frame->references->data[j];
      obj->is_marked = 1;
    }
  }
}


void trace(vm_t *vm){
  if (!vm) return;

  stack_t *gray_obj = stack_new(8);
  if(!gray_obj) return;

  for (int i = 0; i < (int)vm->objects->count; ++i){
    snake_obj_t *obj = vm->objects->data[i];
    if(obj && obj->is_marked) stack_push(gray_obj, obj);
  }

  while (gray_obj->count > 0){
    snake_obj_t *poped  = stack_pop(gray_obj);
    trace_blacken_object(gray_obj, poped);
  }

  stack_free(gray_obj);
}


void trace_blacken_object(stack_t *gray_objects, snake_obj_t *obj) {
  if (!obj) return;

  switch (obj->kind){
    case INTEGER: return;
    case FLOAT: return;
    case STRING: return;
    case VECTOR:
      trace_mark_object(gray_objects,obj->data.v_vector.x);
      trace_mark_object(gray_objects,obj->data.v_vector.y);
      trace_mark_object(gray_objects,obj->data.v_vector.z);
      return;
    case ARRAY:
      snake_array_t arr = obj->data.v_array;
      for (int i = 0; i < (int)arr.size; ++i){
        trace_mark_object(gray_objects, snake_array_get(obj, i));
      }
      return;
    default:
      printf("Some Error occurs\n");
      return;
  }
}


void trace_mark_object(stack_t *gray_objects, snake_obj_t *obj) {
  if (!obj) return;
  if (obj->is_marked) return;

  obj->is_marked = 1;
  stack_push(gray_objects, (void *)obj);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VITUAL MACHINE FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


vm_t *vm_new(void){
  vm_t *vm = (vm_t *) malloc(sizeof(vm_t));
  if(!vm) return NULL;

  vm->frames = stack_new(8);
  if (!vm->frames){
    free(vm);
    return NULL;
  }

  vm->objects = stack_new(8);
  if(!vm->objects){
    stack_free(vm->frames);
    free(vm);
    return NULL;
  }

  return vm;
}


void vm_free(vm_t *vm){
  if (!vm) return;
  for(int i = 0; i < (int)vm->frames->count; ++i){
    frame_free(vm->frames->data[i]);
  }
  stack_free(vm->frames);

  for(int i = 0; i < (int)vm->frames->count; ++i){
    frame_free(vm->objects->data[i]);
  }
  stack_free(vm->objects);

  free(vm);
}


void vm_frame_push(vm_t *vm, frame_t *frame){
  if (!vm || !frame) return;

  stack_push(vm->frames, frame);
}


frame_t *vm_new_frame(vm_t *vm){
  if (!vm) return NULL;

  frame_t *frame = malloc(sizeof(frame_t));
  if(!frame) return NULL;

  frame->references = stack_new(8);
  if(!frame->references){
    free(frame);
    return NULL;
  }

  vm_frame_push(vm, frame);
  return frame;
}


void vm_track_object(vm_t *vm, snake_obj_t *obj){
  if (!vm) return;

  stack_push(vm->objects, (void *)obj);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FRAME FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void frame_free(frame_t *frame){
  if(!frame) return;

  stack_free(frame->references);
  free(frame);
}


void frame_reference_object(frame_t *frame, snake_obj_t *obj){
  if(!frame || !obj) return;

  stack_push(frame->references, (void *)obj);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VITUAL MACHINE SNAKE FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


snake_obj_t *new_vm_snake_obj(vm_t *vm){
  snake_obj_t *obj = calloc(1, sizeof(snake_obj_t));
  if(!obj) return NULL;

  vm_track_object(vm, obj);
  obj->is_marked = 0;
  return obj;
}


snake_obj_t *new_snek_array(vm_t *vm, size_t size) {
  snake_obj_t *obj = new_vm_snake_obj(vm);
  if (obj == NULL) {
    return NULL;
  }

  snake_obj_t **elements = calloc(size, sizeof(snake_obj_t *));
  if (elements == NULL) {
    free(obj);
    return NULL;
  }

  obj->kind = ARRAY;
  obj->data.v_array = (snake_array_t){.size = size, .elements = elements};

  return obj;
}

snake_obj_t *new_snek_vector3(
    vm_t *vm, snake_obj_t *x, snake_obj_t *y, snake_obj_t *z
) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  snake_obj_t *obj = new_vm_snake_obj(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = VECTOR;
  obj->data.v_vector = (snake_vector_t){.x = x, .y = y, .z = z};

  return obj;
}

snake_obj_t *new_snek_integer(vm_t *vm, int value) {
  snake_obj_t *obj = new_vm_snake_obj(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = value;

  return obj;
}

snake_obj_t *new_snek_float(vm_t *vm, float value) {
  snake_obj_t *obj = new_vm_snake_obj(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = FLOAT;
  obj->data.v_float = value;
  return obj;
}

snake_obj_t *new_snek_string(vm_t *vm, char *value) {
  snake_obj_t *obj = new_vm_snake_obj(vm);
  if (obj == NULL) {
    return NULL;
  }

  int len = strlen(value);
  char *dst = malloc(len + 1);
  if (dst == NULL) {
    free(obj);
    return NULL;
  }

  strcpy(dst, value);

  obj->kind = STRING;
  obj->data.v_string = dst;
  return obj;
}

void snek_object_free(snake_obj_t *obj) {
  switch (obj->kind) {
  // Integers and floats are allocated within the object, so nothing to free
  case INTEGER:
  case FLOAT:
    break;
  case STRING:
    // Free the allocated data for the string
    free(obj->data.v_string);
    break;
  case VECTOR: {
    // Since x, y, and z are all allocated within the `snek_object_t`,
    // there is nothing additional for us to free here!
    break;
  }
  case ARRAY: {
    // Free the element list, but not the elements themselves.
    //  The rest of the mark-and-sweep algorithm will take care of that.
    snake_array_t *array = &obj->data.v_array;
    free(array->elements);

    break;
  }
  }

  free(obj);
}

