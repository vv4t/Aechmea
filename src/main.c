#include "common.h"

#include "render.h"
#include "game.h"
#include "window.h"

#include "asset.h"

#include "c_game.h"

gscene_t g_scene_list[] = {
	{ cgame_load, cgame_unload, cgame_render, cgame_update, cgame_event, cgame_remove }
};

typedef enum {
	SCENE_GAME
} g_scene;

void load_assets(asset_t* asset) {
	asset_load_mesh(asset, "asset/mesh/hand.obj");
	asset_load_file(asset, "asset/shader/gui.pixel");
	asset_load_file(asset, "asset/shader/gui.vertex");
	asset_load_file(asset, "asset/shader/light.pixel");
	asset_load_file(asset, "asset/shader/light.vertex");
	asset_load_texture(asset, "asset/tex/map1.png");
	asset_load_texture(asset, "asset/tex/map2.png");
	asset_load_texture(asset, "asset/tex/map3.png");
	asset_load_texture(asset, "asset/tex/map4.png");
	asset_load_texture(asset, "asset/tex/map5.png");
	asset_load_texture(asset, "asset/tex/map6.png");
	asset_load_texture(asset, "asset/tex/map7.png");
	asset_load_texture(asset, "asset/tex/sprites.png");
}

int main(int argc, char* argv[]) {
	win_init(800, 600, "game");
	
	win_input_init();
	
	win_cursor_lock();

	float t;

	game_t game;
	asset_t asset;
	
	asset_init(&asset, mb(4));

	load_assets(&asset);
	
	r_init(kb(360), kb(64));
	
	g_init(&game, &asset, g_scene_list);

	g_load(&game, SCENE_GAME);

	while ( win_loop() ) {
		t = win_time();
		
		win_poll();
		
		g_frame(&game, t);
	}
	
	win_exit();
}
