#include "game.h"

void g_init(game_t* g, asset_t* asset, gscene_t* scene_list) {
	g->t			= 0;
	g->f			= 0;
	
	g->prev			= 0.0f;
	g->delta		= 0.0f;

	g->asset		= asset;
	
	g->scene		= NULL;
	g->scene_list	= scene_list;
}

void g_load(game_t* g, int scene) {
	if (g->scene)
		g_scene_free(g->scene, g->asset);
	
	g->scene = &g->scene_list[scene];

	g_scene_load(g->scene, g->asset);
}

void g_frame(game_t* g, float t) {
	g->current	= t;
	g->elapsed	= g->current - g->prev;
	g->prev		= g->current;
	
	g->delta += g->elapsed;
	
	if (g->delta >= 0.0f) {
		g->delta -= FRAME_TIME;
		
		if (g->t % 60 == 0) {
			com_printf(LOG_DEBUG, "frames: %i", g->f);
			
			g->f = 0;
		}
		
		while (in_poll_event(&g->event))
			g_scene_event(g->scene, g->event);
		
		g_scene_update(g->scene, g->asset, g->t);
		
		g->t++;
	}
	
	g_scene_render(g->scene);
	
	g->f++;
}
