#ifndef OBJECT_H
#define OBJECT_H
#include <stddef.h>


typedef struct snake_obj snake_obj_t ;
typedef struct snake_vector snake_vector_t;

typedef enum snake_obj_kind {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR,
  ARRAY
}snake_obj_kind_t;


typedef struct  snake_array {
  size_t size;
  snake_obj_t **elements;
}snake_array_t;


typedef struct snake_vector{
  snake_obj_t *x;
  snake_obj_t *y;
  snake_obj_t *z;
}snake_vector_t;


typedef union snake_obj_data {
  int v_int;
  float v_float;
  char *v_string;
  snake_vector_t v_vector;
  snake_array_t v_array;
}snake_obj_data_t;



typedef struct snake_obj {
  snake_obj_kind_t kind;
  snake_obj_data_t data;

  // Add is_marked for mark and sweep
  int is_marked; // 0 false, 1 true

  /*
  // Add reference count for refcount algotithm
  int refcount;
  */
}snake_obj_t;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ REFCOUNT FUNCTIONS WITH RAW SNAKE_OBJ IMPLEMENTATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
// Function for refcount algorithm
void refcount_inc(snake_obj_t *obj);
snake_obj_t *new_snake_object();
void refcount_dec(snake_obj_t *obj);
void refcount_free(snake_obj_t *obj);
*/


// Main snake object utils
snake_obj_t *new_snake_int(int value);
snake_obj_t *new_snake_float(float value);
snake_obj_t *new_snake_string(char *string);
snake_obj_t *new_snake_vector(snake_obj_t *x, snake_obj_t *y, snake_obj_t *z);
snake_obj_t *new_snake_array(size_t size);
snake_obj_t *snake_array_get(snake_obj_t *snake_obj, size_t index);
snake_obj_t *snake_add(snake_obj_t *a, snake_obj_t *b);
int snake_array_set(snake_obj_t *snake_obj, size_t index, snake_obj_t *value);
int snake_length(snake_obj_t *obj);



#endif // OBJECT_H
