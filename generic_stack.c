#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// generic swap
void swap(void *val1, void *val2, size_t size);

// Stack with generic pointers
typedef struct Stack {
  size_t count;
  size_t capacity;
  void ** data;
} stack_t;

// new stack function
stack_t *stack_new(size_t capacity);
// stack push function
void stack_push(stack_t *stack, void *obj);
// stack pop function
void *stack_pop(stack_t *stack);
// stack free function
void stack_free(stack_t *stack);

int main(void){
  printf("Starting memory allocation tutorial\n");
  return 0;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SWAP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// generic swap
void swap(void *val1, void *val2, size_t size){
  if (!val1 || !val2 || size < 1) return ; // return in case of incorrect parameters

  void *tmp = (void *) malloc(size);
  if (!tmp) return;

  // tmp = a
  memcpy(tmp, val1, size);

  // a = b
  memcpy(val1, val2, size);

  // b = tmp
  memcpy(val2, tmp, size);

  free(tmp);
}

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
