#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// generic swap
void swap(void *val1, void *val2, size_t size);


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

