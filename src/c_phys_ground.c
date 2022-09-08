#include "c_phys_collider.h"

cphys_collider_t c_phys_ground_init(char* m, int mask, int w, int h, float y) {
	cphys_collider_t c;
		c.type = PHYS_COLLIDER_GROUND;
		c.ground.mask = mask;
		c.ground.m = m;
		c.ground.w = w;
		c.ground.h = h;
		c.ground.y = y;
	
	return c;
}

int c_phys_ground_intersect(cphys_ground_t* g, vec3_t p) {
	int xc = (int) floor(p[0]);
	int yc = (int) floor(p[2]);
	
	if (xc < 0 || yc < 0 || xc >= g->w || yc >= g->h)
		return 1;
	
	return g->m[xc + yc * g->w] & g->mask;
}

void c_phys_ground_move(cphys_collider_t* c, vec3_t v) {
	
}

void c_phys_collide_ground_ground(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	it->d = 0;
	
	return;
}

void c_phys_collide_ground_aabb(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	it->d = 0;
	
	return;
}

void c_phys_collide_ground_map(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	it->d = 0;
	
	return;
}