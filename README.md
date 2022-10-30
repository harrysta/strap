# strap

A C library that wraps strings and arrays into objects for easier manipulation.

## Examples

```c
#include "strap.h"

int main(int argc, char **argv)
{
  StrapString *str = s_string_alloc("string");
  /* "string" */
  s_string_strcat(str, " mystring");
  /* "string mystring" */
  s_string_erase(str, 7, 2);
  /* "string string" */
  s_string_reverse(str);
  /* "gnirts gnirts" */
  
  StrapArray *arr = s_array_alloc(STRAP_TYPE_STRING);
  /* [] */
  arr = s_string_split(str, arr, " ");
  /* ["gnirts", "gnirts"] */
  s_array_append_cstr(arr, "other");
  /* ["gnirts", "gnirts", "other"] */
  s_array_erase(arr, 1);
  /* ["gnirts", "other"] */
  s_array_replace_cstr(arr, 0, "some");
  /* ["some", "other"] */
  
  StrapArray *iarr = s_array_alloc(STRAP_TYPE_INT);
  /* [] */
  int numbers[7] = { 15, 4, 2, 5, 12, 99, 53 };
  s_array_numcpy(iarr, numbers, 5);
  /* [15, 4, 2, 5, 12] */
  int sum = s_array_sum_int(iarr);
  /* 38 */
  
  StrapArray *isubarr = s_array_create_subarray(iarr, 2, 3);
  /* [2, 5, 12] */
  s_array_reverse(isubarr);
  /* [12, 5, 2] */
  char buf[128];
  /* "" */
  s_array_sprintf(isubarr, buf);
  /* "[12, 5, 2]" */
  
  /* Don't forget to clean up! */
  s_string_free(str);
  s_array_free(arr);
  s_array_free(iarr);
  s_array_free(isubarr);
  return 0;
}
```

## Installation

    git clone https://github.com/harrysta/strap
    cd strap
    mkdir build && cd build
    cmake ..
    sudo make install

Using gcc, you can then compile a source file by running
    
    gcc -lstrap -o my_bin my_src.c
