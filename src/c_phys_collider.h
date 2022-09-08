#ifndef C_PHYS_COLLIDER_H
#define C_PHYS_COLLIDER_H

#include "cirno.h"

typedef enum {
	PHYS_COLLIDER_AABB,
	PHYS_COLLIDER_MAP,
	PHYS_COLLIDER_GROUND
} cphys_type_t;

typedef struct {
	vec3_t	b[2];
} cphys_aabb_t;

typedef struct {
	vec3_t	p;

	char*	m;

	int		w;
	int		h;

	int		mask;
} cphys_map_t;

typedef struct {
	char*	m;

	int		w;
	int		h;

	int		mask;

	float	y;
} cphys_ground_t;

typedef struct {
	cphys_type_t type;
	
	union {
		cphys_map_t		map;
		cphys_aabb_t	aabb;
		cphys_ground_t	ground;
	};
} cphys_collider_t;

typedef struct {
	vec3_t	n;
	
	float	d;
} cphys_intersect_t;

int						c_phys_map_intersect(cphys_map_t* map, vec3_t p);

cphys_collider_t		c_phys_map_init(vec3_t p, char* map, int w, int h, int mask);

void					c_phys_map_move(cphys_collider_t* c, vec3_t v);

void					c_phys_collide_map_map(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);

void					c_phys_collide_map_aabb(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);

void					c_phys_collide_map_ground(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);



cphys_collider_t		c_phys_aabb_init(vec3_t a, vec3_t b);

void					c_phys_aabb_move(cphys_collider_t* c, vec3_t v);

void					c_phys_collide_aabb_aabb(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);

void					c_phys_collide_aabb_map(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);

void					c_phys_collide_aabb_ground(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);



int						c_phys_ground_intersect(cphys_ground_t* map, vec3_t p);

cphys_collider_t		c_phys_ground_init(char* m, int mask, int w, int h, float y);

void					c_phys_ground_move(cphys_collider_t* c, vec3_t v);

void					c_phys_collide_ground_ground(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);

void					c_phys_collide_ground_aabb(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);

void					c_phys_collide_ground_map(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b);

#endif
