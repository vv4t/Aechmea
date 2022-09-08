#include "c_local.h"

vec3_t c_player_aabb[] = {
	{ -0.25f, -0.7f, -0.25f },
	{  0.25f,  0.0f,  0.25f },
};

vec3_t c_player_hand_pos = { 0.1f, -0.2f, 0.2f };

vec4_t c_player_light_color = { 1, 1, 1, 2 };

const int c_player_atk_cd = 4;

void c_player_move(gentity_t* p, cphys_t* pm, vec3_t cmd, float accel, float speed) {
	if (cmd[0] || cmd[2]) {
		vec3_t v;

		vec3_rotate(cmd, p->rot, v);
		v[1] = 0.0f;

		vec3_normalize(v, v);
		
		c_phys_accelerate(pm, v, accel, speed);
	}

	if (pm->grounded && cmd[1]) {
		vec3_t v = { 0.0f, 2.4f, 0.0f };

		c_phys_add_force(pm, v);

		cmd[1] = 0;
	}
}

void c_player_init(cplayer_t* player, gscene_t* scene, asset_t* asset,
						grender_t* render, gphys_t* phys, gsprite_t* sprite, gbullet_t* bullet,
						ghealth_t* health) {
	
	asset_mesh_t* mesh = asset_find_mesh(asset, "asset/mesh/hand.obj");

	player->p = g_scene_add_entity(scene);
		vec3_set(player->p->pos, 2.0f, 0.8f, 2.0f);
		player->pm = g_phys_add_rigidbody(phys, player->p, 1.0f,
											c_phys_aabb_init(c_player_aabb[0], c_player_aabb[1]));
	
	player->hand = g_scene_add_entity(scene);
		g_render_add(render, player->hand, r_add_mesh(mesh->vertices, mesh->size, NULL, 0));
		vec3_mulf(player->hand->scale, 0.2f, player->hand->scale);
	
	player->p->tag = C_PLAYER;

	player->scene = scene;
	player->sprite = sprite;
	player->bullet = bullet;
	player->render = render;
	player->phys = phys;
	player->health = health;

	player->health->hp[g_scene_entity_id(scene, player->p)] = 5.0f;

	player->prev_hp = player->health->hp[g_scene_entity_id(scene, player->p)];

	player->light = g_render_light_add(render, player->hand->pos, c_player_light_color);
}

void c_player_update(cplayer_t* player, cinput_t* input, int t) {
	vec3_t dir = { 0.0f, 0.0f, 15.0f };

	cbullet_t* b;

	cphys_t* phys;

	c_lock(player->p, player->hand, c_player_hand_pos);
	
	float accel = player->pm->grounded ? 0.1f : 1.0f;
	float speed = player->pm->grounded ? 8.0f : 0.6f;

	float player_hp = player->health->hp[g_scene_entity_id(player->scene, player->p)];

	if (player->prev_hp > player_hp) {
		player->slowed = 10;

		player->prev_hp = player_hp;
	}

	if (player->slowed > 0) {
		speed = 0.2f;

		player->slowed--;
	}

	if (player->slowed < 0) {
		speed = 10.0f;

		player->slowed++;
	}

 	c_player_move(player->p, player->pm, input->axis, accel, speed);

	if (player->pm->rigidbody_collision) {
		gentity_t* entity = player->pm->rigidbody_collision->entity;

		if (entity->tag == C_HEALTH) {
			player->slowed -= 90;

			player->health->hp[g_scene_entity_id(player->scene, player->p)] += 2.0f;

			player->prev_hp = player->health->hp[g_scene_entity_id(player->scene, player->p)];

			if (player->health->hp[g_scene_entity_id(player->scene, player->p)] > 5.0f)
				player->health->hp[g_scene_entity_id(player->scene, player->p)] = 5.0f;
			
			csprite_t* spr;
			
			for (int i = 0; i < player->sprite->pool.length; i++) {
				if (!pool_is_alloc(&player->sprite->pool, i))
					continue;
				
				spr = pool_get(&player->sprite->pool, i);

				if (spr->entity == entity) {
					spr->v = 2;

					break;
				}
			}

			g_phys_remove_rigidbody(player->phys, entity);
		}
	}

	if (input->attack[0] && t > player->atk_next) {
		gentity_t* bullet = g_scene_add_entity(player->scene);
			vec3_mulf(bullet->scale, 0.7f, bullet->scale);

			g_sprite_add(player->sprite, bullet, 1, 2, 2);
			
			vec3_rotate(dir, player->p->rot, dir);
			vec3_copy(bullet->pos, player->hand->pos);
			quat_copy(bullet->rot, player->p->rot);

			b = g_bullet_add(player->bullet, bullet, dir, C_BULLET_PLAYER, 0.1f, 0.1f, 30);

		player->atk_next = t + c_player_atk_cd;
	}

	vec3_t v = { 0, 0, 2 };

	vec3_rotate(v, player->p->rot, v);

	if (v[1] < -0.5) {
		float f = -0.5 - v[1];
		
		vec3_mulf(v, 1-f, v);
	}

	vec3_add(v, player->p->pos, v);

	g_render_light_update(player->render, player->light, v, c_player_light_color);
}