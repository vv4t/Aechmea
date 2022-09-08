#ifndef R_LOCAL_H
#define R_LOCAL_H

#define VERTEX_ATTRIB_0		3
#define VERTEX_ATTRIB_1		3
#define VERTEX_ATTRIB_2		2

#define VERTEX_SIZE			32

#define GLEW_STATIC

#include "common.h"

#include "render.h"

#include <GL/glew.h>

void r_init_buffer(int VERTEX_BUFFER_SIZE, int INDEX_BUFFER_SIZE);

#endif
