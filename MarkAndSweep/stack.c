#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ STACK FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stack_t *stack_new(size_t capacity){
  if (capacity < 1) return NULL;

  stack_t *new = (stack_t *) malloc(sizeof(stack_t));
  if (!new) return NULL;

  new->count = 0;
  new->capacity = capacity;
  new->data = (void **) malloc(sizeof(void *) * capacity);
  if (!new->data) {
    free(new);
    return NULL;
  }

  return new;
}


void stack_push(stack_t *stack, void *obj){
  if(!stack || !obj) return;

  // check if count is eq to capacity
  if(stack->capacity == stack->count){
    stack->capacity *= 2;
    void **temp = realloc(stack->data, sizeof(void *) * stack->capacity);
    if(!temp) return;
    stack->data = temp;
  }

  // add object
  *(stack->data + stack->count) = obj;
  stack->count++;
}


void *stack_pop(stack_t *stack){
  if(!stack || stack->count == 0) return NULL;

  stack->count--;
  return *(stack->data + stack->count);
}


void stack_free(stack_t *stack){
  if(!stack) return;
  if(stack->data) free(stack->data);

  free(stack);
  stack = NULL;
}


void stack_remove_nulls(stack_t *stack) {
  size_t new_count = 0;

  // Iterate through the stack and compact non-NULL pointers.
  for (size_t i = 0; i < stack->count; ++i) {
    if (stack->data[i] != NULL) {
      stack->data[new_count++] = stack->data[i];
    }
  }

  // Update the count to reflect the new number of elements.
  stack->count = new_count;

  // Optionally, you might want to zero out the remaining slots.
  for (size_t i = new_count; i < stack->capacity; ++i) {
    stack->data[i] = NULL;
  }
}
