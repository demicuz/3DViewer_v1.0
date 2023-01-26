#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>

#define array_clean(_arr) ((_arr) ? free(_array_header(_arr)), 0 : 0)
#define array_push(_arr, _val)                                         \
  (_array_mgrow(_arr, 1)                                               \
       ? ((_arr)[_array_size(_arr)++] = (_val), _array_size(_arr) - 1) \
       : 0)
#define array_size(_arr) ((_arr) ? _array_size(_arr) : 0)
#define array_reset_size(_arr) ((_arr) ? _array_reset_size(_arr) : 0)
#define array_capacity(_arr) ((_arr) ? _array_capacity(_arr) : 0)
#define is_array_empty(_arr) (array_size(_arr) == 0)

#define _array_header(_arr) ((unsigned int *)(_arr)-2)
#define _array_size(_arr) (_array_header(_arr)[0])
#define _array_reset_size(_arr) (_array_header(_arr)[0] = 0)
#define _array_capacity(_arr) (_array_header(_arr)[1])
#define _array_ngrow(_arr, _n) \
  ((_arr) == 0 || (_array_size(_arr) + (_n) >= _array_capacity(_arr)))
#define _array_mgrow(_arr, _n) \
  (_array_ngrow(_arr, _n) ? (_array_grow(_arr, _n) != 0) : 1)
#define _array_grow(_arr, _n) \
  (*((void **)&(_arr)) = array_realloc(_arr, _n, sizeof(*(_arr))))

void *array_realloc(void *ptr, unsigned int n, size_t elem_size);

#endif
