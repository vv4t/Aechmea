#ifndef G_LOCAL_H
#define G_LOCAL_H

#include "common.h"

#include "input.h"

#include "asset.h"

#include "cirno.h"

#include "g_scene.h"

static const float FRAME_TIME = 1.0f / 60.0f;

typedef struct {
	float		prev;
	float		delta;
	float		current;
	float		elapsed;
	
	int			t;
	int			f;
	
	asset_t*	asset;

	gscene_t*	scene;
	gscene_t*	scene_list;
	
	in_event_t*	event;
} game_t;

void	g_init(game_t* g, asset_t* asset, gscene_t* scene_list);

void	g_load(game_t* g, int scene);

void	g_frame(game_t* g, float t);

#endif
