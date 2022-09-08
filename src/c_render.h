#ifndef C_RENDER_H
#define C_RENDER_H

#include "render.h"

#include "game.h"

#define PX_SPRITE_W 1.0f / 4.0f
#define PX_SPRITE_H 1.0f / 8.0f

typedef struct {
	mat4_t				transform;
	mat4_t				model;
	
	gentity_t*			entity;

	int					size;
	int					offset;
	
	r_mesh_t			mesh;
} crender_t;

typedef struct {
	vec4_t	pos;
	
	vec4_t	color;
} clight_t;

typedef struct {
	pool_t		pool;
	pool_t		pool_light;
	
	r_block_t	vblk;
} grender_t;

void		g_render_init(grender_t* render, memhunk_t* hunk, int pool_size, int pool_light_size);

void		g_render_update(grender_t* render, mat4_t cam);

void		g_render_render(grender_t* render);

void		g_render_attach_shader(grender_t* render, r_shader_t shader, const char* name);

crender_t*	g_render_add(grender_t* render, gentity_t* entity, r_mesh_t mesh);


clight_t*	g_render_light_add(grender_t* render, vec3_t pos, vec4_t color);

void		g_render_light_update(grender_t* render, clight_t* light, vec3_t pos, vec4_t color);

void		g_render_light_remove(grender_t* render, clight_t* light);


void		g_render_update_dead(grender_t* render, int dead);

#endif
