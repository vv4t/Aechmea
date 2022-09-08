#include "c_phys_collider.h"

cphys_collider_t c_phys_map_init(vec3_t p, char* map, int w, int h, int mask) {
	cphys_collider_t c;
		c.type = PHYS_COLLIDER_MAP;
		c.map.mask = mask;
		c.map.m = map;
		c.map.w = w;
		c.map.h = h;
	
	return c;
}

void c_phys_map_move(cphys_collider_t* c, vec3_t v) {

}

void c_phys_collide_map_map(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	it->d = 0;
	return;
}

void c_phys_collide_map_aabb(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	it->d = 0;
	return;
}

void c_phys_collide_map_ground(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	it->d = 0;
	
	return;
}

int c_phys_map_intersect(cphys_map_t* map, vec3_t p) {
	int xc = (int) floor(p[0]);
	int yc = (int) floor(p[2]);
	
	if (xc < 0 || yc < 0 || xc >= map->w || yc >= map->h)
		return 1;
	
	return map->m[xc + yc * map->w] & map->mask;
}