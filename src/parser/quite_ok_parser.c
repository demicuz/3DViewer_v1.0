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

char *skip_digits(char *s) {
  while (*s >= '0' && *s <= '9') {
    s++;
  }

  return s;
}

char *skip_texture_and_normals(char *s) {
  if (*s == '/') {
    s++;
  }
  s = skip_digits(s);
  if (*s == '/') {
    s++;
  }
  s = skip_digits(s);

  return s;
}

void print_error(const char *error, t_parser *p) {
  (void)fprintf(stderr, "%s:%zd: error: %s\n", p->basename, p->lines_read + 1,
                error);
}

bool parse_vertex(t_parser *p, GLfloat **vertices, t_bbox *bb) {
  for (int i = 0; i < 3; ++i) {
    char *new_ptr;
    errno = 0;
    GLfloat coord = strtof(p->cursor, &new_ptr);
    if (errno != 0) {
      print_error(strerror(errno), p);
      return false;
    }
    if (p->cursor == new_ptr) {
      print_error("missing vertex coordinates", p);
      return false;
    }
    p->cursor = new_ptr;

    array_push(*vertices, coord);

    // update bbox
    // bb->raw is [x_min, x_max, y_min, y_max, z_min, z_max]
    // i = 0 is x, i = 1 is y, i = 2 is z
    if (coord < bb->raw[(ptrdiff_t)i * 2]) {
      bb->raw[(ptrdiff_t)i * 2] = coord;
    } else if (coord > bb->raw[i * 2 + 1]) {
      bb->raw[i * 2 + 1] = coord;
    }
  }

  return true;
}

bool parse_index(t_parser *p, GLuint *result) {
  char *new_ptr;
  errno = 0;
  *result = strtoul(p->cursor, &new_ptr, 10);
  if (errno != 0) {
    print_error(strerror(errno), p);
    return false;
  }
  if (p->cursor == new_ptr) {
    return false;
  }

  p->cursor = skip_texture_and_normals(new_ptr);

  return true;
}

// TODO GLuint boundary check
// TODO face is defined by at least 3 indices
// TODO relative indices
bool parse_face(t_parser *p, GLuint **lines) {
  GLuint first_i;
  if (!parse_index(p, &first_i)) {
    print_error("no vertex indices", p);
    return false;
  }

  array_push(*lines, first_i);

  GLuint i;
  while (parse_index(p, &i)) {
    array_push(*lines, i);
    array_push(*lines, i);
  }

  array_push(*lines, first_i);

  return true;
}

bool parse_line(t_parser *p, t_object *obj) {
  p->cursor = skip_whitespace(p->cursor);

  if (starts_with("v ", p->cursor) || starts_with("v\t", p->cursor)) {
    p->cursor += 2;
    if (!parse_vertex(p, &obj->vertices, &obj->bbox)) {
      return false;
    }
  } else if (starts_with("f ", p->cursor) || starts_with("f\t", p->cursor)) {
    p->cursor += 2;
    if (!parse_face(p, &obj->indices)) {
      return false;
    }
  }

  return true;
}

bool read_error(FILE *file, char buffer[], t_parser *p) {
  if (ferror(file)) {
    print_error("IO corruption while reading file", p);
    return true;
  }

  if (strlen(buffer) == MAX_LINE_LEN - 1) {
    print_error("line is too long!", p);
    return true;
  }

  return false;
}

bool parse_obj(const char *filepath, t_object *obj) {
  FILE *file = fopen(filepath, "rb");
  if (!file) {
    perror("fopen");
    return false;
  }

  t_parser p = {0};
  char buffer[MAX_LINE_LEN];
  p.basename = strrchr(filepath, '/') + 1;

  array_reset_size(obj->vertices);
  obj->vertex_count = 0;
  array_reset_size(obj->indices);
  obj->edge_count = 0;
  obj->bbox = (t_bbox){0};

  // A dummy vertex to not offset all the indices from .obj
  array_push(obj->vertices, 0);
  array_push(obj->vertices, 0);
  array_push(obj->vertices, 0);

  while (fgets(buffer, MAX_LINE_LEN, file)) {
    if (read_error(file, buffer, &p)) {
      (void)fclose(file);
      return false;
    }

    p.cursor = buffer;
    if (!parse_line(&p, obj)) {
      (void)fclose(file);
      return false;
    }

    p.lines_read++;
  }

  obj->vertex_count = array_size(obj->vertices) / 3 - 1;
  // TODO: Count true number of edges.
  // Currently, this is just a hack. We're assuming that every edge belongs
  // exactly to two faces. So we're dividing by 2 to get true line count that
  // we're gonna draw, and then again by 2 to get "edge count".
  obj->edge_count = array_size(obj->indices) / 4;

  (void)fclose(file);
  return true;
}
