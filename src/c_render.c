#include "c_render.h"

#include <string.h>

enum {
	CBLOCK_MODEL		= 0,
	CBLOCK_TRANSFORM	= 64,
	CBLOCK_DEAD			= 128,
	CBLOCK_LIGHT		= 144,
} cblock_location_t;

void g_render_init(grender_t* render, memhunk_t* hunk, int pool_size, int pool_light_size) {
	int block_size = 144 + pool_light_size * sizeof(clight_t);

	int c = 0;
	
	render->vblk = r_add_block(block_size);

	for (int i = 0; i < pool_light_size; i++) {
		r_block_sub_data(&c, CBLOCK_LIGHT + i * sizeof(clight_t) + 7 * sizeof(float), sizeof(int));
	}

	hunk_pool_alloc(hunk, &render->pool, pool_size, sizeof(crender_t));

	hunk_pool_alloc(hunk, &render->pool_light, pool_light_size, sizeof(clight_t));
}

crender_t* g_render_add(grender_t* render, gentity_t* entity, r_mesh_t mesh) {
	crender_t* r = pool_alloc(&render->pool);
		r->entity	= entity;
		r->mesh		= mesh;
		r->size		= r_mesh_size(mesh);
		r->offset	= 0;
	
	mat4_identity(r->model);

	return r;
}

void g_render_update(grender_t* render, mat4_t cam) {
	mat4_t m;
	
	crender_t* r;
	gentity_t* entity;
	
	for (int i = 0; i < render->pool.length; i++) {
		r = pool_get(&render->pool, i);
		entity = r->entity;

		if (entity) {
			mat4_identity(r->model);
			
			mat4_translate(m, entity->pos);
			mat4_mul(r->model, m, r->model);
			
			mat4_rotate(m, entity->rot);
			mat4_mul(r->model, m, r->model);
			
			mat4_scale(m, entity->scale);
			mat4_mul(r->model, m, r->model);
			
			mat4_mul(cam, r->model, r->transform);
		} else {
			mat4_identity(r->model);
			mat4_copy(cam, r->transform);
		}
	}
}

void g_render_render(grender_t* render) {
	crender_t* r;
	
	for (int i = 0; i < render->pool.length; i++) {
		r = pool_get(&render->pool, i);
		
		r_bind_block(render->vblk);

		r_block_sub_data(r->model, CBLOCK_MODEL, sizeof(mat4_t));
		r_block_sub_data(r->transform, CBLOCK_TRANSFORM, sizeof(mat4_t));
		
		r_draw_mesh(r->mesh, r->offset, r->size);
	}
}

clight_t* g_render_light_add(grender_t* render, vec3_t pos, vec4_t color) {
	clight_t* light = pool_alloc(&render->pool_light);
	
	g_render_light_update(render, light, pos, color);

	return light;
}

void g_render_light_update(grender_t* render, clight_t* light, vec3_t pos, vec4_t color) {
	vec3_copy(light->pos, pos);
	quat_copy(light->color, color);

	r_block_sub_data(light, CBLOCK_LIGHT + ((char*) light - (char*) render->pool_light.blk), sizeof(clight_t));
}

void g_render_attach_shader(grender_t* render, r_shader_t shader, const char* name) {
	r_uniform_block(shader, name, render->vblk);
}

void g_render_update_dead(grender_t* render, int dead) {
	r_block_sub_data(&dead, CBLOCK_DEAD, sizeof(int));
}