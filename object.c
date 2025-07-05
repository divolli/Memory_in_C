#include "object.h"
#include <stdlib.h>
#include <string.h>


snake_obj_t *new_snake_int(int value){
  snake_obj_t *new = (snake_obj_t *) malloc(sizeof(snake_obj_t));
  if (!new) return NULL;

  new->data.v_int = value;
  new->kind = INTEGER;
  return new;
}


snake_obj_t *new_snake_float(float value){
  snake_obj_t *new = (snake_obj_t *) malloc(sizeof(snake_obj_t));
  if (!new) return NULL;

  new->data.v_float = value;
  new->kind = FLOAT;
  return new;
}


snake_obj_t *new_snake_string(char *string){
  snake_obj_t *new = (snake_obj_t *) malloc(sizeof(snake_obj_t));
  if (!new) return NULL;

  new->data.v_string = strdup(string);
  if (!new->data.v_string){
    free(new);
    return NULL;
  }
  new->kind = STRING;
  return new;
}


snake_obj_t *new_snake_vector(snake_obj_t *x, snake_obj_t *y, snake_obj_t *z){
  if (!x || !y || !z) return NULL;
  snake_obj_t *new = (snake_obj_t *) malloc(sizeof(snake_obj_t));
  if (!new) return NULL;

  new->kind = VECTOR;
  new->data.v_vector.x = x;
  new->data.v_vector.y = y;
  new->data.v_vector.z = z;

  return new;
}


snake_obj_t *new_snake_array(size_t size){
  if (size < 1) return NULL;

  snake_obj_t *new = (snake_obj_t *)malloc(sizeof(snake_obj_t));
  if (!new) return NULL;

  new->kind = ARRAY;
  new->data.v_array.size = size;
  new->data.v_array.elements = (snake_obj_t **) calloc(size, sizeof(snake_obj_t *));
  if (!new->data.v_array.elements){
    free(new);
    return NULL;
  }

  return new;
}


int snake_array_set(snake_obj_t *snake_obj, size_t index, snake_obj_t *value){
  // reutrns 0 if false and 1 if true
  if (!snake_obj || !value) return 0;
  if (snake_obj->kind != ARRAY) return 0;
  if (snake_obj->data.v_array.size <= index) return 0;

  *(snake_obj->data.v_array.elements + index) = value;
  return 1;
}


snake_obj_t *snake_array_get(snake_obj_t *snake_obj, size_t index){
  if (!snake_obj || snake_obj->kind != ARRAY) return NULL;
  if (snake_obj->data.v_array.size <= index) return NULL;

  return *(snake_obj->data.v_array.elements + index);
}


int snake_length(snake_obj_t *obj){
  if (!obj) return -1;

  switch(obj->kind){
    case INTEGER : return 1;
    case FLOAT : return 1;
    case STRING : return strlen(obj->data.v_string);
    case VECTOR : return 3;
    case ARRAY : return obj->data.v_array.size;
    default : return -1;
  }

}


snake_obj_t *snake_add(snake_obj_t *a, snake_obj_t *b){

}
