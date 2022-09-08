#ifndef C_GAME_H
#define C_GAME_H

#include "c_local.h"

typedef enum {
	CG_LEVEL_SCENE1,
	CG_LEVEL_SCENE2,
	CG_LEVEL_SCENE3,
	CG_LEVEL_SCENE4,
	CG_LEVEL_SCENE5,
	CG_LEVEL_SCENE6,
	CG_LEVEL_SCENE7
} cg_level_t;

typedef enum {
	CG_SHADER_GUI,
	CG_SHADER_LIGHT
} cg_shader_t;

typedef struct {
	ghealth_t	health;
	
	gphys_t		phys;
	grender_t	render;
	gsprite_t	sprite;
	gbullet_t	bullet;
	gdirector_t	director;
	ggui_t		gui;

	cview_t		view;
	cinput_t	input;
	cmap_t		map;
	cplayer_t	player;

	const char* level[7];

	r_shader_t	shader[2];

	int			dead_time;

	int			restart;

	int			gui_health_bar;

	cg_level_t	current_level;
} cgame_t;

void	cgame_load(gscene_t* scene, asset_t* asset);

void	cgame_unload(gscene_t* scene, asset_t* asset);

void	cgame_render(gscene_t* scene);

void	cgame_update(gscene_t* scene, asset_t* asset, int t);

void	cgame_event(gscene_t* scene, in_event_t* event);

void	cgame_remove(gscene_t* scene, gentity_t* entity);

#endif
