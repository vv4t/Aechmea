#ifndef CM_LOCAL_H
#define CM_LOCAL_H

#include "cirno.h"
#include "memory.h"
#include "input.h"

#include "asset.h"

typedef struct {
	vec3_t	pos;
	vec3_t	scale;
	
	quat_t	rot;

	int		tag;
} gentity_t;

typedef struct gscene_t {
	void		(*load)(struct gscene_t* g, asset_t* asset);
	void		(*free)(struct gscene_t* g, asset_t* asset);
	void		(*render)(struct gscene_t* g);
	void		(*update)(struct gscene_t* g, asset_t* asset, int update);
	void		(*event)(struct gscene_t* g, in_event_t* event);
	void		(*remove)(struct gscene_t* g, gentity_t* entity);
	
	void*		d;
	
	pool_t		pool;
	
	memhunk_t	hunk;
} gscene_t;

void				g_scene_load(gscene_t* scene, asset_t* asset);

void				g_scene_free(gscene_t* scene, asset_t* asset);

void				g_scene_update(gscene_t* scene, asset_t* asset, int t);

void				g_scene_render(gscene_t* scene);

void				g_scene_event(gscene_t* sscene, in_event_t* event);

void				g_scene_alloc_entity_pool(gscene_t* scene, int size);

gentity_t*			g_scene_add_entity(gscene_t* scene);

void				g_scene_remove_entity(gscene_t* scene, gentity_t* entity);

int					g_scene_entity_id(gscene_t* scene, gentity_t* entity);

#endif
