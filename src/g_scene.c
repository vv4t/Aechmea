#include "g_scene.h"

void g_scene_load(gscene_t* scene, asset_t* asset) {
	scene->load(scene, asset);
}

void g_scene_free(gscene_t* scene, asset_t* asset) {
	scene->free(scene, asset);
}

void g_scene_render(gscene_t* scene) {
	scene->render(scene);
}

void g_scene_update(gscene_t* scene, asset_t* asset, int t) {
	scene->update(scene, asset, t);
}

void g_scene_event(gscene_t* scene, in_event_t* event) {
	scene->event(scene, event);
}

void g_scene_alloc_entity_pool(gscene_t* scene, int size) {
	hunk_pool_alloc(&scene->hunk, &scene->pool, size, sizeof(gentity_t));
}

gentity_t* g_scene_add_entity(gscene_t* scene) {
	gentity_t* entity = pool_alloc(&scene->pool);
		vec3_set(entity->scale, 1.0, 1.0, 1.0);
		
		vec3_init(entity->pos);
		quat_init(entity->rot);

	entity->tag = -1;
	
	return entity;
}

void g_scene_remove_entity(gscene_t* scene, gentity_t* entity) {
	scene->remove(scene, entity);

	pool_remove(&scene->pool, entity - (gentity_t*) scene->pool.blk);
}

int g_scene_entity_id(gscene_t* scene, gentity_t* entity) {
	return entity - (gentity_t*) scene->pool.blk;
}