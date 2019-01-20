/**
 File: test_file.c
 Enter a description for this file.
*/
#include "cs136.h"
#include "array.h"

bool distinct_range(const int a[], int len) {
  assert(a);
  assert(len>0&& len<=9);
  int b[len];
  for(int i=0; i<len; ++i) {
    b[i] = 0;
  }
  for(int i=0; i<len; ++i) {
    if (a[i]>=1 && a[i] <= 9){
      b[a[i]-1]++;
    }
  }
  for(int j=0; j<len; ++j) {
    if (b[j] >1) {
      return false;
    }
  }
  return true;
}

int main(void) {
int a[] = {0, 0, 3, 4};
  int b[] = {1, 2, 3, 4, 5, 6};
  assert(distinct_range(a, 4));
  assert(distinct_range(b, 6));
}
