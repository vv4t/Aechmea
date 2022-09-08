#include "cirno.h"

#include "common.h"

float lmaxf(float a, float b) {
	return a > b ? a : b;
}

float lclampf(float c, float a, float b) {
	if (c < a) c = a;
	if (c > b) c = b;
	
	return c;
}

void vec3_printf(vec3_t v) {
	com_printf(LOG_DEBUG, "%f %f %f", v[0], v[1], v[2]);
}

void vec3_init(vec3_t v) {
	memset(v, 0, sizeof(vec3_t));
}

void vec3_copy(vec3_t a, vec3_t b) {
	memcpy(a, b, sizeof(vec3_t));
}

void vec3_set(vec3_t v, float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void vec3_add(vec3_t a, vec3_t b, vec3_t c) {
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}

void vec3_sub(vec3_t a, vec3_t b, vec3_t c) {
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

void vec3_mul(vec3_t a, vec3_t b, vec3_t c) {
	c[0] = a[0] * b[0];
	c[1] = a[1] * b[1];
	c[2] = a[2] * b[2];
}

void vec3_div(vec3_t a, vec3_t b, vec3_t c) {
	c[0] = a[0] / b[0];
	c[1] = a[1] / b[1];
	c[2] = a[2] / b[2];
}


void vec3_addf(vec3_t a, float b, vec3_t c) {
	c[0] = a[0] + b;
	c[1] = a[1] + b;
	c[2] = a[2] + b;
}

void vec3_subf(vec3_t a, float b, vec3_t c) {
	c[0] = a[0] - b;
	c[1] = a[1] - b;
	c[2] = a[2] - b;
}

void vec3_mulf(vec3_t a, float b, vec3_t c) {
	c[0] = a[0] * b;
	c[1] = a[1] * b;
	c[2] = a[2] * b;
}

void vec3_divf(vec3_t a, float b, vec3_t c) {
	c[0] = a[0] / b;
	c[1] = a[1] / b;
	c[2] = a[2] / b;
}

void vec3_cross(vec3_t a, vec3_t b, vec3_t c) {
	float ax = a[0];
	float ay = a[1];
	float az = a[2];
	
	float bx = b[0];
	float by = b[1];
	float bz = b[2];
	
	c[0] = ay * bz - az * by;
	c[1] = az * bx - ax * bz;
	c[2] = ax * by - ay * bx;
}

void vec3_round(vec3_t a, vec3_t b) {
	b[0] = round(a[0]);
	b[1] = round(a[1]);
	b[2] = round(a[2]);
}

float vec3_maxf(vec3_t v) {
	float f = -1024.0f;
	
	for (int i = 0; i < 3; i++)
		f = v[i] > f ? v[i] : f;
	
	return f;
}

void vec3_max(vec3_t a, vec3_t b, vec3_t c) {
	for (int i = 0; i < 3; i++)
		c[i] = a[i] > b[i] ? a[i] : b[i];
}

float vec3_dot(vec3_t a, vec3_t b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void vec3_rotate(vec3_t v, quat_t q, vec3_t a) {
	quat_t c;
	quat_t b;
	
	quat_conjugate(q, b);
	
	quat_vec3_mul(q, v, c);
	quat_mul(c, b, c);
	
	vec3_copy(a, c);
}

void vec3_normalize(vec3_t v, vec3_t n) {
	float d = vec3_length(v);
	
	if (d == 0) return;
	
	n[0] = v[0] / d;
	n[1] = v[1] / d;
	n[2] = v[2] / d;
}

vec_t vec3_length(vec3_t v) {
	float d = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	
	return sqrt(d);
}



void quat_printf(quat_t q) {
	com_printf(LOG_DEBUG, "%f %f %f %f", q[0], q[1], q[2], q[3]);
}

void quat_init(quat_t q) {
	memset(q, 0, sizeof(quat_t));
	
	q[3] = 1; 
}

void quat_copy(quat_t a, quat_t b) {
	memcpy(a, b, sizeof(quat_t));
}

void quat_set(quat_t v, float x, float y, float z, float w) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

void quat_rotate(quat_t q, vec3_t v, float angle) {
	float sine		= sin(angle / 2.0f);
	float cosine	= cos(angle / 2.0f);
	
	q[0] = v[0] * sine;
	q[1] = v[1] * sine;
	q[2] = v[2] * sine;
	q[3] = cosine;
}

void quat_mul(quat_t a, quat_t b, quat_t c) {
	vec_t x = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
	vec_t y = a[3] * b[1] + a[1] * b[3] + a[2] * b[0] - a[0] * b[2];
	vec_t z = a[3] * b[2] + a[2] * b[3] + a[0] * b[1] - a[1] * b[0];
	vec_t w = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
	
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = w;
}

void quat_vec3_mul(quat_t a, vec3_t b, quat_t c) {
	vec_t x =  a[3] * b[0] + a[1] * b[2] - a[2] * b[1];
	vec_t y =  a[3] * b[1] + a[2] * b[0] - a[0] * b[2];
	vec_t z =  a[3] * b[2] + a[0] * b[1] - a[1] * b[0];
	vec_t w = -a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
	
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = w;
}

void quat_conjugate(quat_t a, quat_t b) {
	b[0] = -a[0];
	b[1] = -a[1];
	b[2] = -a[2];
	b[3] =  a[3];
}

void quat_normalize(quat_t q, quat_t n) {
	vec_t d = quat_length(q);
	
	if (d == 0) return;
	
	n[0] = q[0] / d;
	n[1] = q[1] / d;
	n[2] = q[2] / d;
	n[3] = q[3] / d;
}

vec_t quat_length(quat_t q) {
	float d = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	
	return sqrt(d);
}


void mat4_printf(mat4_t m) {
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0)
			printf("\n");
		
		printf("%f ", m[i]);
	}
	
	printf("\n");
}

void mat4_copy(mat4_t a, mat4_t b) {
	memcpy(b, a, sizeof(mat4_t));
}

void mat4_identity(mat4_t m) {
	m[0]	= 1;	m[4]	= 0;	m[8]	= 0;	m[12]	= 0;
	m[1]	= 0;	m[5]	= 1;	m[9]	= 0;	m[13]	= 0;
	m[2]	= 0;	m[6]	= 0;	m[10]	= 1;	m[14]	= 0;
	m[3]	= 0;	m[7]	= 0;	m[11]	= 0;	m[15]	= 1;
}

void mat4_rotate(mat4_t m, quat_t q) {
	vec_t x = q[0];
	vec_t y = q[1];
	vec_t z = q[2];
	vec_t w = q[3];
	
	m[0]	= 1-2*y*y - 2*z*z;	m[4]	= 2*x*y - 2*z*w;	m[8]	= 2*x*z + 2*y*w;	m[12]	= 0;
	m[1]	= 2*x*y + 2*z*w;	m[5]	= 1-2*x*x - 2*z*z;	m[9]	= 2*y*z - 2*x*w;	m[13]	= 0;
	m[2]	= 2*x*z - 2*y*w;	m[6]	= 2*y*z + 2*x*w;	m[10]	= 1-2*x*x - 2 *y*y;	m[14]	= 0;
	m[3]	= 0;				m[7]	= 0;				m[11]	= 0;				m[15]	= 1;
}

void mat4_scale(mat4_t m, vec3_t v) {
	m[0]	= v[0];	m[4]	= 0;	m[8]	= 0;	m[12]	= 0;
	m[1]	= 0;	m[5]	= v[1];	m[9]	= 0;	m[13]	= 0;
	m[2]	= 0;	m[6]	= 0;	m[10]	= v[2];	m[14]	= 0;
	m[3]	= 0;	m[7]	= 0;	m[11]	= 0;	m[15]	= 1;
}

void mat4_translate(mat4_t m, vec3_t v) {
	m[0]	= 1;	m[4]	= 0;	m[8]	= 0;	m[12]	= v[0];
	m[1]	= 0;	m[5]	= 1;	m[9]	= 0;	m[13]	= v[1];
	m[2]	= 0;	m[6]	= 0;	m[10]	= 1;	m[14]	= v[2];
	m[3]	= 0;	m[7]	= 0;	m[11]	= 0;	m[15]	= 1;
}

void mat4_perspective(mat4_t m, float ar, float fov, float n, float f) {
	m[0]	= 1/(ar*tan(fov/2));	m[4]	= 0;			m[8]	= 0;			m[12]	= 0;
	m[1]	= 0;					m[5]	= 1/tan(fov/2);	m[9]	= 0;			m[13]	= 0;
	m[2]	= 0;					m[6]	= 0;			m[10]	= (-n-f)/(n-f);	m[14]	= (2*f*n)/(n-f);
	m[3]	= 0;					m[7]	= 0;			m[11]	= 1;			m[15]	= 0;
}

void mat4_mul(mat4_t a, mat4_t b, mat4_t c) {
	mat4_t r;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float sum = 0.0f;
			
			for (int k = 0; k < 4; k++)
				sum += b[i * 4 + k] * a[k * 4 + j];
			
			r[i * 4 + j] = sum;
		}
	}
	
	memcpy(c, r, sizeof(mat4_t));
}
