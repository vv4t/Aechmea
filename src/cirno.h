#ifndef CIRNO_H
#define CIRNO_H

/*
|                                    |
|    Cirno's Perfect Math Library    |
|                                    |
*/

#define _USE_MATH_DEFINES

#include <math.h>

#define lmin(a, b) a < b ? a : b
#define lmax(a, b) a > b ? a : b

typedef float	vec_t;

typedef vec_t	vec2_t[2];
typedef vec_t	vec3_t[3];
typedef vec_t	vec4_t[4];
typedef vec_t	quat_t[4];

typedef vec_t	mat4_t[16];

float			lmaxf(float a, float b);

float			lclampf(float c, float a, float b);

void			vec3_printf(vec3_t v);

void			vec3_init(vec3_t v);

void			vec3_copy(vec3_t a, vec3_t b);

void			vec3_set(vec3_t v, float x, float y, float z);

void			vec3_add(vec3_t a, vec3_t b, vec3_t c);

void			vec3_sub(vec3_t a, vec3_t b, vec3_t c);

void			vec3_mul(vec3_t a, vec3_t b, vec3_t c);

void			vec3_div(vec3_t a, vec3_t b, vec3_t c);

void			vec3_addf(vec3_t a, float b, vec3_t c);

void			vec3_subf(vec3_t a, float b, vec3_t c);

void			vec3_mulf(vec3_t a, float b, vec3_t c);

void			vec3_divf(vec3_t a, float b, vec3_t c);

float			vec3_maxf(vec3_t v);

void			vec3_max(vec3_t a, vec3_t b, vec3_t c);

void			vec3_cross(vec3_t a, vec3_t b, vec3_t c);

void			vec3_round(vec3_t a, vec3_t b);

float			vec3_dot(vec3_t a, vec3_t b);

void			vec3_rotate(vec3_t v, quat_t q, vec3_t a);

void			vec3_normalize(vec3_t v, vec3_t n);

vec_t			vec3_length(vec3_t v);



void			quat_printf(quat_t q);

void			quat_init(quat_t q);

void			quat_copy(quat_t a, quat_t b);

void			quat_set(quat_t q, float x, float y, float z, float w);

void			quat_rotate(quat_t q, vec3_t v, float angle);

void			quat_mul(quat_t a, quat_t b, quat_t c);

void			quat_vec3_mul(quat_t a, quat_t b, vec3_t c);

void			quat_conjugate(quat_t a, quat_t b);

void			quat_normalize(quat_t q, quat_t n);

vec_t			quat_length(quat_t q);


void			mat4_printf(mat4_t m);

void			mat4_copy(mat4_t a, mat4_t b);

void			mat4_identity(mat4_t m);

void			mat4_scale(mat4_t m, vec3_t v);

void			mat4_rotate(mat4_t m, quat_t q);

void			mat4_translate(mat4_t m, vec3_t v);

void			mat4_perspective(mat4_t m, float aspect_ratio, float fov, float near, float far);

void			mat4_mul(mat4_t a, mat4_t b, mat4_t c);

#endif
