#include "3DViewer.h"
#include "vector.h"
#include "dynamic-array.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glad/gl.h>
#include <errno.h>

#define MAX_LINE_LEN 65536

void *array_realloc(void *ptr, unsigned int n, size_t elem_size) {
  unsigned int sz = array_size(ptr);
  unsigned int nsz = sz + n;
  unsigned int cap = array_capacity(ptr);
  unsigned int ncap = 3 * cap / 2;
  unsigned int *r;

  if (ncap < nsz)
    ncap = nsz;
  ncap = (ncap + 15) & ~15u;

  r = realloc(ptr ? _array_header(ptr) : 0, elem_size * ncap + 2 * sizeof(unsigned int));
  if (!r)
    return 0;

  r[0] = sz;
  r[1] = ncap;

  return (r + 2);
}

bool starts_with(const char *start, const char *s)
{
  while (*start && *s && *start == *s)
  {
    start++;
    s++;
  }
  return (!*start);
}

char *skip_whitespace(char *s) {
  while (*s == ' ' || *s == '\n' || *s == '\v' || *s == '\f' || *s == '\r'
    || *s == '\t') {
    s++;
  }

  return s;
}

bool parse_vertex(char **ptr, GLfloat **vertices, t_bbox *bb) {
  // printf("trying to parse vertex: %s\n", *ptr);

  for (int i = 0; i < 3; ++i) {
    *ptr = skip_whitespace(*ptr);
    char *new_ptr;
    errno = 0;
    GLfloat coord = strtof(*ptr, &new_ptr);
    if (errno != 0) {
      perror("strtof");
      return false;
    }
    if (*ptr == new_ptr) {
      (void)fprintf(stderr, "Error: missing vertex coordinates\n");
      return false;
    }
    // printf("parsed: %f\n", coord);
    // printf("rest: %s\n", new_ptr);
    *ptr = new_ptr;
    array_push(*vertices, coord);

    // update bbox
    // bb->raw is [x_min, x_max, y_min, y_max, z_min, z_max]
    // i = 0 is x, i = 1 is y, i = 2 is z
    if (coord < bb->raw[(ptrdiff_t)i * 2]) {
      bb->raw[i] = coord;
    } else if (coord > bb->raw[i * 2 + 1]) {
      bb->raw[i * 2 + 1] = coord;
    }
  }

  return true;
}

// TODO GLuint boundary check
// TODO face is defined by at least 3 indices
// TODO handle `/` characters
bool parse_face(char **ptr, GLuint **lines) {
  // printf("trying to parse face: %s\n", *ptr);
  char *new_ptr;
  errno = 0;
  GLuint first_i = strtoul(*ptr, &new_ptr, 10);
  if (errno != 0) {
    perror("strtol");
    return false;
  }
  if (*ptr == new_ptr) {
    (void)fprintf(stderr, "Error: missing face index\n");
    return false;
  }
  // printf("parsed: %d\n", first_i);
  // printf("rest: %s\n", new_ptr);
  *ptr = new_ptr;
  array_push(*lines, first_i);
  *ptr = skip_whitespace(*ptr);

  while (**ptr) {
    // char *new_ptr;
    errno = 0;
    GLuint i = strtoul(*ptr, &new_ptr, 10);
    if (errno != 0) {
      perror("strtol");
      return false;
    }
    if (*ptr == new_ptr) {
      (void)fprintf(stderr, "Error: missing face index\n");
      return false;
    }
    // printf("parsed: %d\n", i);
    // printf("rest: %s\n", new_ptr);
    *ptr = new_ptr;
    array_push(*lines, i);
    array_push(*lines, i);
    *ptr = skip_whitespace(*ptr);
  }
  array_push(*lines, first_i);

  return true;
}

bool parse_obj(const char *file_path, t_object *obj) {
  FILE *file = fopen(file_path, "rb");
  char buffer[MAX_LINE_LEN];
  size_t lines_read = 0;

  array_clean(obj->vertices);
  array_clean(obj->indices);
  obj->vertices = NULL;
  obj->indices = NULL;
  obj->bbox = (t_bbox){0};

  if (!file) {
    perror("fopen");
    return false;
  }

  // A dummy vertex to not offset all the indices from .obj
  array_push(obj->vertices, 0);
  array_push(obj->vertices, 0);
  array_push(obj->vertices, 0);

  errno = 0;
  while (fgets(buffer, MAX_LINE_LEN, file)) {
    if (ferror(file)) {
      (void)fclose(file);
      return false;
    }
    if (strlen(buffer) == MAX_LINE_LEN - 1) {
      (void)fprintf(stderr, ".obj file: line %zd is too long!\n", lines_read + 1);
      (void)fclose(file);
      return false;
    }
    // printf("line read: %s\n", buffer);
    char *ptr = buffer;

    ptr = skip_whitespace(ptr);
    if (starts_with("v ", ptr) || starts_with("v\t", ptr)) {
      ptr += 2;
      if (!parse_vertex(&ptr, &obj->vertices, &obj->bbox)) {
        // TODO fail function
        (void)fprintf(stderr, ".obj file error on line %zd\n", lines_read + 1);
        (void)fclose(file);
        return false;
      }
    } else if (starts_with("f ", ptr) || starts_with("f\t", ptr)) {
      ptr += 2;
      if (!parse_face(&ptr, &obj->indices)) {
        // TODO fail function
        (void)fprintf(stderr, ".obj file error on line %zd\n", lines_read + 1);
        (void)fclose(file);
        return false;
      }
    }
    lines_read++;
  }

  if (errno) {
    perror("getline");
    (void)fclose(file);
    return false;
  }

  (void)fclose(file);
  return true;
}
