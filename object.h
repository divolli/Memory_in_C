#ifndef OBJECT_H
#define OBJECT_H

typedef struct snake_obj snake_obj_t ;
typedef struct snake_vector snake_vector_t;

typedef enum snake_obj_kind {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR
}snake_obj_kind_t;


typedef union snake_obj_data {
  int v_int;
  float v_float;
  char *v_string;
  snake_vector_t v_vector;
}snake_obj_data_t;


typedef struct snake_obj {
  snake_obj_kind_t kind;
  snake_obj_data_t data;
}snake_obj_t;


typedef struct snake_vector{
  snake_obj_t *x;
  snake_obj_t *y;
  snake_obj_t *z;
}snake_vector_t;


snake_obj_t *new_snake_int(int value);
snake_obj_t *new_snake_float(float value);
snake_obj_t *new_snake_string(char *string);
snake_obj_t *new_snake_vector();

#endif // OBJECT_H
