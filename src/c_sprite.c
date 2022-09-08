#include "c_local.h"

void g_sprite_init(gsprite_t* sprite, memhunk_t* hunk, grender_t* render, int p_spr) {
	r_mesh_t mesh;
	
	mesh = r_alloc_mesh(p_spr * 4, p_spr * 6);

	sprite->mesh = g_render_add(render, NULL, mesh);

	sprite->mesh->size = 0;

	hunk_pool_alloc(hunk, &sprite->pool, p_spr, sizeof(csprite_t)); 
	
	int* indices = malloc(p_spr * 6 * sizeof(int));

	for (int i = 0; i < p_spr; i++) {
		int a = i * 4;

		for (int j = 0; j < 6; j++)
			indices[i * 6 + j] = a + j % 3 + j / 3;
	}

	r_mesh_sub_index(sprite->mesh->mesh, indices, 0, p_spr * 6);

	free(indices);
}

csprite_t* g_sprite_add(gsprite_t* sprite, gentity_t* entity, int free_rot, int u, int v) {
	csprite_t* spr = pool_alloc(&sprite->pool);
		spr->entity = entity;
		spr->free_rot = free_rot;
		spr->u = u;
		spr->v = v;

	return spr;
}

void g_sprite_remove(gsprite_t* sprite, gentity_t* entity) {
	csprite_t* spr;
	
	for (int i = 0; i < sprite->pool.length; i++) {
		if (!pool_is_alloc(&sprite->pool, i))
			continue;
		
		spr = pool_get(&sprite->pool, i);

		if (spr->entity == entity) {
			pool_remove(&sprite->pool, spr - (csprite_t*) sprite->pool.blk);

			break;
		}
	}
}

void g_sprite_update(gsprite_t* sprite, vec3_t pos) {
	csprite_t* s;
	
	int xc, yc;

	float vertices[32];
	float theta, cosine, sine;	
	float* v;

	vec3_t delta;

	int s_len = 0;

	float px = 1.0 / PX_SPRITE_W / 64.0;
	float py = 1.0 / PX_SPRITE_H / 64.0;

	vec3_t up = { 0, 1, 0 };

	for (int i = 0; i < sprite->pool.length; i++) {
		if (!pool_is_alloc(&sprite->pool, i))
			continue;

		s = pool_get(&sprite->pool, i);

		if (!s->free_rot) {
			vec3_sub(s->entity->pos, pos, delta);

			theta = atan2(delta[0], delta[2]);

			quat_rotate(s->entity->rot, up, theta);
		}

		for (int j = 0; j < 4; j++) {
			v = &vertices[j * 8];

			xc = j % 2; yc = j / 2;

			vec3_set(v, xc, yc, 0);

			v[0] -= 0.5f; v[1] -= 0.5f;

			vec3_mul(v, s->entity->scale, v);
			
			vec3_rotate(v, s->entity->rot, v);
			vec3_add(v, s->entity->pos, v);

			vec3_init(&v[3]);

			v[6] = xc * (PX_SPRITE_W - 2 * px) + px + s->u * PX_SPRITE_W;
			v[7] = (1 - yc) * (PX_SPRITE_H - 2 * py) + py + s->v * PX_SPRITE_H;
		}

		r_mesh_sub_vertex(sprite->mesh->mesh, vertices, s_len * 4, 4);

		s_len++;
	}

	sprite->mesh->size = s_len * 6;
}
