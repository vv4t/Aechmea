#include "c_phys_collider.h"

#include "common.h"

cphys_collider_t c_phys_aabb_init(vec3_t a, vec3_t b) {
	cphys_collider_t col;
		col.type = PHYS_COLLIDER_AABB;
		
		vec3_copy(col.aabb.b[0], a);
		
		vec3_copy(col.aabb.b[1], b);
	
	return col;
}

void c_phys_aabb_move(cphys_collider_t* c, vec3_t v) {
	vec3_add(c->aabb.b[0], v, c->aabb.b[0]);
	
	vec3_add(c->aabb.b[1], v, c->aabb.b[1]);
}

void c_phys_collide_aabb_aabb(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	vec3_t dist, dist1, dist2;
	
	float d;
	vec3_t n;
	
	vec3_init(n);
	
	vec3_sub(b->aabb.b[0], a->aabb.b[1], dist1);
	vec3_sub(a->aabb.b[0], b->aabb.b[1], dist2);
		
	vec3_max(dist1, dist2, dist);
	
	d = vec3_maxf(dist);
	
	for (int i = 0; i < 3; i++)
		if (dist[i] == d) n[i] = (d < 0) * 2 - 1;
	
	vec3_copy(it->n, n);
	
	it->d = d;
}

void c_phys_collide_aabb_map(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	vec3_t p, d, c;

	float m = 0;

	vec3_init(it->n);
	
	for (int i = 0; i < 4; i++) {
		p[0] = (a->aabb.b[i % 2])[0];
		p[2] = (a->aabb.b[i / 2])[2];

		if (c_phys_map_intersect(&b->map, p)) {
			vec3_round(p, c);

			vec3_sub(p, c, d);

			int a = 2 * (fabs(d[0]) > fabs(d[2]));

			if (-fabs(d[a]) < m && d[a]) {
				vec3_init(it->n);

				it->n[a] = -d[a] / fabs(d[a]);

				m = -fabs(d[a]);
			}
		}
	}

	it->d = m;
}

void c_phys_collide_aabb_ground(cphys_intersect_t* it, cphys_collider_t* a, cphys_collider_t* b) {
	float ymin, ymax;

	vec3_t p;

	ymin = fminf(a->aabb.b[0][1], a->aabb.b[1][1]);
	ymax = fmaxf(a->aabb.b[0][1], a->aabb.b[1][1]);
	
	vec3_set(it->n, 0, 1, 0);
	
	it->d = 0;

	if (ymin + (ymax - ymin) * 0.1 > b->ground.y) {
		for (int i = 0; i < 4; i++) {
			p[0] = (a->aabb.b[i % 2])[0];
			p[2] = (a->aabb.b[i / 2])[2];

			if (c_phys_ground_intersect(&b->ground, p)) {

				it->d = ymin - b->ground.y;

			}
		}
	}
}