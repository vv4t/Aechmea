#include "c_game.h"

void cg_load(gscene_t* scene, asset_t* asset, cg_level_t cg_level) {
	cgame_t* g = scene->d;

	if (cg_level > CG_LEVEL_SCENE7)
		exit(-1);

	asset_tex_t* map = asset_find_texture(asset, g->level[cg_level]);

	g_scene_alloc_entity_pool(scene, 64);
	 
	g_gui_init(&g->gui, 4);
	g_phys_init(&g->phys, &scene->hunk, scene, 9.81f, 64, 4);
	g_render_init(&g->render, &scene->hunk, 4, 48);
	g_sprite_init(&g->sprite, &scene->hunk, &g->render, 128);
	g_bullet_init(&g->bullet, &scene->hunk, scene, &g->phys, &g->health, 64); 
	g_director_init(&g->director, &scene->hunk, scene, &g->phys, &g->sprite, &g->bullet, &g->health, 48);

	c_view_init(&g->view);
	c_player_init(&g->player, scene, asset, &g->render, &g->phys, &g->sprite, &g->bullet, &g->health);
	c_map_init(&g->map, scene, &g->render, &g->director, &g->phys, map->pixels, map->w, map->h);

	float ar = 640.0f / 480.0f;

	mat4_perspective(g->view.p, ar, 1.57f, 0.1f, 20.0f);

	g_render_attach_shader(&g->render, g->shader[CG_SHADER_LIGHT], "block");

	g_render_update_dead(&g->render, 0);

	r_color(0.1f, 0.05, 0.05);

	float cur_sz = 0.025f;

	g_gui_add_rect(&g->gui, 0.5f - cur_sz / 2, 0.5f - cur_sz * ar / 2,
					cur_sz, cur_sz * ar, 3, 4, 1, 1);

	g->gui_health_bar = g_gui_add_rect(&g->gui, 0.05, 0.97, 0.7f, -0.03f, 3, 1, 1, 1);

	g->dead_time = 0;
	g->restart = 0;
}

void cg_load_level(gscene_t* scene, asset_t* asset) {
	cgame_t* g = scene->d;

	g->level[CG_LEVEL_SCENE1] = "asset/tex/map1.png";
	g->level[CG_LEVEL_SCENE2] = "asset/tex/map2.png";
	g->level[CG_LEVEL_SCENE3] = "asset/tex/map3.png";
	g->level[CG_LEVEL_SCENE4] = "asset/tex/map4.png";
	g->level[CG_LEVEL_SCENE5] = "asset/tex/map5.png";
	g->level[CG_LEVEL_SCENE6] = "asset/tex/map6.png";
	g->level[CG_LEVEL_SCENE7] = "asset/tex/map7.png";
}

void cg_load_shader(gscene_t* scene, asset_t* asset) {
	cgame_t* g = scene->d;

	asset_file_t *pixel, *vertex;

	pixel = asset_find_file(asset, "asset/shader/gui.pixel");
	vertex = asset_find_file(asset, "asset/shader/gui.vertex");

	g->shader[CG_SHADER_GUI] = r_add_shader(vertex->buffer, pixel->buffer);

	pixel = asset_find_file(asset, "asset/shader/light.pixel");
	vertex = asset_find_file(asset, "asset/shader/light.vertex");

	g->shader[CG_SHADER_LIGHT] = r_add_shader(vertex->buffer, pixel->buffer);
}

void cg_restart(gscene_t* scene, asset_t* asset) {
	cgame_t* g = scene->d;

	hunk_reset(&scene->hunk, scene->hunk.block + sizeof(cgame_t));

	r_mesh_reset(0);
	r_block_reset(0);

	vec3_set(g->player.p->pos, 2.0f, 3.0f, 2.0f);
}

void cg_on_dead(gscene_t* scene) {
	cgame_t* g = scene->d;

	r_color(0.5f, 0.05, 0.05);

	g_render_update_dead(&g->render, 1);

	g->player.pm->collider.aabb.b[0][1] += 0.5f;

	g->player.hand->pos[1] = -1;
}

void cg_dead_update(gscene_t* scene, asset_t* asset, int t) {
	cgame_t* g = scene->d;
	
	if (t < 30) {
		vec3_t axis = { 1, 0, 1 };

		quat_t q;

		quat_rotate(q, axis, t / 200.0f);

		quat_mul(g->view.rot, q, g->view.rot);
	}

	if (g->restart) {
		cg_restart(scene, asset);
		cg_load(scene, asset, g->current_level);
	}
}

void cgame_load(gscene_t* scene, asset_t* asset) {
	cgame_t* g;

	asset_tex_t* sprites = asset_find_texture(asset, "asset/tex/sprites.png");

	hunk_init(&scene->hunk, kb(26));

	g = scene->d = hunk_alloc(&scene->hunk, sizeof(cgame_t));

	memset(g, 0, sizeof(cgame_t));
	
	cg_load_level(scene, asset);
	cg_load_shader(scene, asset);

	g->current_level = CG_LEVEL_SCENE1;

	r_bind_texture(r_add_texture(sprites->pixels, sprites->w, sprites->h), 0);

	cg_load(scene, asset, g->current_level);
}

void cgame_unload(gscene_t* scene, asset_t* asset) {
	cgame_t* g = scene->d;
}

void cgame_update(gscene_t* scene, asset_t* asset, int t) {
	cgame_t* g = scene->d;

	quat_copy(g->player.p->rot, g->view.rot);
	vec3_copy(g->view.pos, g->player.p->pos);

	if (g->player.pm->rigidbody_collision) {
		gentity_t* entity = g->player.pm->rigidbody_collision->entity;

		if (entity->tag == C_PORTAL) {
			cg_restart(scene, asset);
			cg_load(scene, asset, ++g->current_level);

			return;
		}
	}

	float player_health = g->health.hp[g_scene_entity_id(scene, g->player.p)];

	if (player_health > 0 && g->player.p->pos[1] > -10.0f && !g->dead_time) {
		g_gui_update(&g->gui, g->gui_health_bar, 0.05, 0.97, 0.7f * player_health / 5.0f, -0.03f, 3, 1, 1 * player_health / 5.0f, 1);

		c_view_free_look(&g->view, &g->input);

		c_player_update(&g->player, &g->input, t);

		g_director_update(&g->director, &g->player, t);
	} else if (!g->dead_time) {
		g_gui_update(&g->gui, g->gui_health_bar, 0, 0, 0, 0, 0, 0, 0, 0);

		cg_on_dead(scene);

		g->dead_time = t;
	} else {
		cg_dead_update(scene, asset, t - g->dead_time);

		if (g->restart)
			return;
	}

	c_view_update(&g->view);
	
	g_phys_simulate(&g->phys, 1.0f / 60.0f, 4);
	g_bullet_update(&g->bullet);
	g_sprite_update(&g->sprite, g->view.pos);
	g_render_update(&g->render, g->view.m);

	g->input.yaw = 0;
	g->input.pitch = 0;
}

void cgame_render(gscene_t* scene) {
	cgame_t* g = scene->d;

	r_clear(R_COLOR | R_DEPTH);

	r_bind_shader(g->shader[CG_SHADER_LIGHT]);

	g_render_render(&g->render);

	r_clear(R_DEPTH);

	r_bind_shader(g->shader[CG_SHADER_GUI]);

	g_gui_render(&g->gui);
}

void cgame_event(gscene_t* scene, in_event_t* event) {
	cgame_t* g = scene->d;

	c_input_event(&g->input, event);

	if (event->type == IN_KEYDOWN && event->kbutton == KEY_R && g->dead_time > 0) {
		g->restart = 1;
	}
}

void cgame_remove(gscene_t* scene, gentity_t* entity) {
	cgame_t* g = scene->d;

	g_director_remove(&g->director, entity);
	g_sprite_remove(&g->sprite, entity);
	g_bullet_remove(&g->bullet, entity);
	g_phys_remove_rigidbody(&g->phys, entity);
}

void c_lock(gentity_t* a, gentity_t* b, vec3_t v) {
	vec3_copy(b->pos, v);
	vec3_rotate(b->pos, a->rot, b->pos);

	vec3_add(b->pos, a->pos, b->pos);
	
	quat_copy(b->rot, a->rot);
}
