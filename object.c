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
