#include "object.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
  if (!a || !b) return NULL;

  switch (a->kind){
    case INTEGER:
      switch (b->kind){
        case INTEGER: return new_snake_int(a->data.v_int + b->data.v_int);
        case FLOAT : return new_snake_float((float)a->data.v_int + b->data.v_float);
        default:
          printf("Invalid operation\n");
          return NULL;
      }
    case FLOAT :
      switch (b->kind){
        case INTEGER: return new_snake_float(a->data.v_float + (float)b->data.v_int);
        case FLOAT : return new_snake_float(a->data.v_float + b->data.v_float);
        default:
          printf("Invalid operation\n");
          return NULL;
      }
    case STRING :
      if (b->kind != STRING){
          printf("Invalid operation\n");
          return NULL;
      }

      size_t length = strlen(a->data.v_string) + strlen(b->data.v_string) + 1; // +1 for \0
      char *new_string = (char *) malloc(sizeof(char) * length);
      strcat(new_string, a->data.v_string);
      strcat(new_string, b->data.v_string);
      // not returning just new string beacause we need free previous alloc string
      snake_obj_t *str_obj = new_snake_string(new_string);
      free(new_string);
      return str_obj;

    case VECTOR:
      if (b->kind != VECTOR){
          printf("Invalid operation\n");
          return NULL;
      }
      return new_snake_vector(
        snake_add(a->data.v_vector.x, b->data.v_vector.x),
        snake_add(a->data.v_vector.y, b->data.v_vector.y),
        snake_add(a->data.v_vector.z, b->data.v_vector.z)
        );
    case ARRAY:
      if (b->kind != ARRAY){
          printf("Invalid operation\n");
          return NULL;
      }
      size_t arr_size = a->data.v_array.size + b->data.v_array.size;
      snake_obj_t *arr_obj = new_snake_array(arr_size);

      for(int i = 0; i < a->data.v_array.size; ++i){
        snake_array_set(
          arr_obj,
          i,
          snake_array_get(a, i)
        );
      }

      for(int i = 0; i < a->data.v_array.size; ++i){
        snake_array_set(
          arr_obj,
          i + a->data.v_array.size,
          snake_array_get(b, i)
        );
      }
      return arr_obj;
    default :
      printf("Invalid operation\n");
      return NULL;
  }
}
