#include "c_local.h"

vec3_t c_ghost_aabb[] = {
	{ -0.25f, -0.5f, -0.25f },
	{  0.25f,  0.0f,  0.25f },
};

vec3_t c_ghost_reaper_spawn[] = {
	{ 6, 1.1, 6 },
	{ 10, 1.1, 5 },
	{ 14, 1.1, 6 },
	{ 15, 1.1, 10 },
	{ 14, 1.1, 14 },
	{ 10, 1.1, 15 },
	{ 6, 1.1, 14 }
};

int c_ghost_reaper_minion_count = 0;

void c_ghost_mage_shoot(cghost_t* g, gdirector_t* director, cplayer_t* player, int t) {
	if (t % 50 == 0) {
		vec3_t dir;
		quat_t q;

		vec3_t v;

		vec3_t up = { 0, 1, 0 };

		for (int i = 0; i < 3; i++) {
			quat_rotate(q, up, M_PI + (i - 1) * 0.2f);
			vec3_copy(v, player->p->pos);
			vec3_sub(g->entity->pos, v, dir);

			dir[1] = 0;

			vec3_normalize(dir, dir);
			vec3_mulf(dir, 8.0f, dir);
			vec3_rotate(dir, q, dir);

			gentity_t* p = g_scene_add_entity(director->scene);
				g_sprite_add(director->sprite, p, 1, 2, 0);

				vec3_copy(p->pos, g->entity->pos);
				quat_copy(p->rot, g->entity->rot);

				quat_mul(p->rot, q, p->rot);

				g_bullet_add(director->bullet, p, dir, C_BULLET_GHOST, 0.1f, 0.3f, 20);
		}
	}
}

void c_ghost_orb_shoot(cghost_t* g, gdirector_t* director, cplayer_t* player, int t) {
	if (t % 50 < 18 && t % 6 == 0) {
		vec3_t dir;
		quat_t q;

		vec3_t v;

		vec3_t up = { 0, 1, 0 };

		quat_rotate(q, up, M_PI);
		vec3_copy(v, player->p->pos);
		vec3_sub(v, g->entity->pos, dir);

		dir[1] = 0;

		vec3_normalize(dir, dir);
		vec3_mulf(dir, 7.0f, dir);

		gentity_t* p = g_scene_add_entity(director->scene);
			g_sprite_add(director->sprite, p, 1, 2, 4);

			vec3_copy(p->pos, g->entity->pos);
			quat_copy(p->rot, g->entity->rot);

			p->scale[0] = 0.6f;
			p->scale[1] = 0.6f;

			quat_mul(p->rot, q, p->rot);

			g_bullet_add(director->bullet, p, dir, C_BULLET_GHOST, 0.06f, 0.2f, 50);
	}
}

void c_ghost_necromancer_shoot(cghost_t* g, gdirector_t* director, cplayer_t* player, int t) {
	c_ghost_orb_shoot(g, director, player, t);

	if (t % 30 == 0) {
		vec3_t dir;
		quat_t q;

		vec3_t v;

		vec3_t up = { 0, 1, 0 };

		for (int i = 0; i < 6; i++) {
			quat_rotate(q, up, i * M_PI / 3.0);
			vec3_copy(v, player->p->pos);
			vec3_sub(g->entity->pos, v, dir);

			dir[1] = 0;

			vec3_normalize(dir, dir);
			vec3_mulf(dir, 7.0f, dir);
			vec3_rotate(dir, q, dir);

			gentity_t* p = g_scene_add_entity(director->scene);
				g_sprite_add(director->sprite, p, 1, 0, 6);

				vec3_copy(p->pos, g->entity->pos);
				quat_copy(p->rot, g->entity->rot);

				quat_mul(p->rot, q, p->rot);

				g_bullet_add(director->bullet, p, dir, C_BULLET_GHOST, 0.1f, 0.4f, 10);
		}
	}
}

void c_ghost_warrior_shoot(cghost_t* g, gdirector_t* director, cplayer_t* player, int t) {
	if (t % 120 < 60) {
		g->spr->u = 2 + (t % 60 > 30);
	}

	if (t % 120 == 30) {
		vec3_t dir;
		quat_t q;

		vec3_t v;

		vec3_t up = { 0, 1, 0 };

		for (int i = 0; i < 3; i++) {
			quat_rotate(q, up, M_PI + (i - 1) * 0.3f);
			vec3_copy(v, player->p->pos);
			vec3_sub(g->entity->pos, v, dir);

			dir[1] = 0;

			vec3_normalize(dir, dir);
			vec3_mulf(dir, 5.0f, dir);
			vec3_rotate(dir, q, dir);

			gentity_t* p = g_scene_add_entity(director->scene);
				g_sprite_add(director->sprite, p, 1, 0, 6);

				vec3_copy(p->pos, g->entity->pos);
				quat_copy(p->rot, g->entity->rot);

				quat_mul(p->rot, q, p->rot);

				g_bullet_add(director->bullet, p, dir, C_BULLET_GHOST, 0.1f, 1.0f, 50);
		}
	}
}

void c_ghost_reaper_shoot(cghost_t* g, gdirector_t* director, cplayer_t* player, int t) {

}

void c_ghost_reaper_update(gdirector_t* director, cplayer_t* player, cghost_t* ghost, int t) {
	vec3_t delta;
	vec3_t v;

	vec3_t up = {0,1,0};

	float reaper_hp = director->health->hp[g_scene_entity_id(director->scene, ghost->entity)];

	vec3_sub(ghost->entity->pos, player->p->pos, delta);
	
	delta[1] = 0;
	
	vec3_normalize(delta, delta);

	vec3_mulf(delta, 6.0f, delta);
	
	ghost->spr->u = (t % 60) > 30;
	
	if (reaper_hp == 45.0)
		return;

	if (reaper_hp > 40.0) {
		if (t % 60 == 0) {
			for (int i = -1; i < 2; i++) {
				quat_t q;

				quat_rotate(q, up, M_PI + i * 0.4f);

				gentity_t* p = g_scene_add_entity(director->scene);
					g_sprite_add(director->sprite, p, 1, 1, 6);

					vec3_copy(p->pos, ghost->entity->pos);
					quat_copy(p->rot, ghost->entity->rot);

					vec3_rotate(delta, q, v);

					p->pos[1] = 0.4f;

					g_bullet_add(director->bullet, p, v, C_BULLET_REAPER, 0.1f, 2.0f, 80);
			}
		}
	}
	
	if (reaper_hp < 40.0) {
		
		if (t % 180 == 0 && c_ghost_reaper_minion_count == 0) {
			cghostType_t gtype = rand() % 3;

			for (int i = 0; i < 6; i++) {
				c_ghost_reaper_minion_count++;

				g_director_add_ghost(director, gtype, c_ghost_reaper_spawn[i]);
			}
		}
	}

	if (reaper_hp < 15.0) {
		if (t % 120 < 60)
			ghost->spr->u = 2;

		if (t % 120 < 60 && t % 10 == 0) {
			for (int i = 0; i < 4; i++) {
				quat_t q;

				quat_rotate(q, up, i * M_PI / 2.0 + ((t % 60) / 60.0) * 0.2 + 0.4);

				gentity_t* p = g_scene_add_entity(director->scene);
					g_sprite_add(director->sprite, p, 1, 1, 6);

					vec3_copy(p->pos, ghost->entity->pos);
					quat_copy(p->rot, ghost->entity->rot);

					vec3_rotate(delta, q, v);

					p->pos[1] = 0.4f;

					g_bullet_add(director->bullet, p, v, C_BULLET_REAPER, 0.1f, 2.0f, 80);
			}
		}
	}

	if (director->health->hp[g_scene_entity_id(director->scene, ghost->entity)] <= 0) {
		g_scene_remove_entity(director->scene, ghost->entity);
	}
}

typedef struct {
	int spr;
	
	float hp;

	float speed;
	float dist;
	float shoot_dist;

	void (*shoot)(cghost_t* g, gdirector_t* director, cplayer_t* player, int t);
} cghost_attrib_t;

cghost_attrib_t c_ghost_attrib[5] = {
	{ 0, 1.5f, 0.1f, 6.0f, 8.0f,  c_ghost_mage_shoot		},
	{ 4, 1.0f, 0.5f, 6.0f, 10.0f, c_ghost_orb_shoot			},
	{ 5, 3.0f, 0.2f, 6.0f, 4.0f, c_ghost_warrior_shoot		},
	{ 7, 1.5f, 0.2f, 7.0f, 10.0f, c_ghost_necromancer_shoot	},
	{ 3, 45.0f, 0.0f, 0.0f, 0.0f, c_ghost_reaper_shoot		}
};

void g_director_init(gdirector_t* director, memhunk_t* hunk, gscene_t* scene, gphys_t* phys,
						gsprite_t* sprite, gbullet_t* bullet, ghealth_t* health, int size) {
	director->scene = scene;

	director->phys = phys;
	director->sprite = sprite;
	director->bullet = bullet;
	director->health = health;

	director->enemy_count = 0;
	director->exit_created = 0;

	hunk_pool_alloc(hunk, &director->pool, size, sizeof(cghost_t));
}

void g_director_add_ghost(gdirector_t* director, cghostType_t type, vec3_t p) {
	cghost_t* ghost = pool_alloc(&director->pool);
		ghost->entity = g_scene_add_entity(director->scene);

		ghost->entity->tag = C_GHOST;

		vec3_copy(ghost->entity->pos, p);
		vec3_copy(ghost->pos, p);

		ghost->entity->pos[1] = 1.1;

		if (type == C_GHOST_REAPER) {
			ghost->entity->scale[0] = 1.9f;
			ghost->entity->scale[1] = 1.9f;

			ghost->pm = g_phys_add_rigidbody(director->phys, ghost->entity, 1.0f, c_phys_aabb_init(c_ghost_aabb[0], c_ghost_aabb[1]));

			ghost->pm->gravity = 0;

		} else {
			ghost->pm = g_phys_add_rigidbody(director->phys, ghost->entity, 1.0f, c_phys_aabb_init(c_ghost_aabb[0], c_ghost_aabb[1]));
		}

		ghost->spr = g_sprite_add(director->sprite, ghost->entity, 0, 0, 0);
		ghost->spr->u = 0;
		ghost->spr->v = c_ghost_attrib[type].spr;
		ghost->t = rand() % 60;

		ghost->type = type;

		director->health->hp[ghost->entity - (gentity_t*) director->scene->pool.blk] = c_ghost_attrib[type].hp;
	
	director->enemy_count++;
}

void g_director_add_health(gdirector_t* director, vec3_t p) {
	gentity_t* crystal = g_scene_add_entity(director->scene);
		vec3_copy(crystal->pos, p);

		crystal->pos[1] = 1.1;
		
		g_sprite_add(director->sprite, crystal, 0, 3, 0);
		g_phys_add_rigidbody(director->phys, crystal, 1.0f,
								c_phys_aabb_init(c_ghost_aabb[0], c_ghost_aabb[1]));
		
		crystal->tag = C_HEALTH;
}

void g_director_update(gdirector_t* director, cplayer_t* player, int t) {
	vec3_t p;
	vec3_t p2;

	cghost_t* ghost;

	if (!director->enemy_count) {
		if (!director->exit_created) {
			gentity_t* exit = g_scene_add_entity(director->scene);
				vec3_copy(exit->pos, director->exit);

				exit->pos[1] = 0.6;
				
				g_sprite_add(director->sprite, exit, 0, 3, 3);
				g_phys_add_rigidbody(director->phys, exit, 1.0f,
										c_phys_aabb_init(c_ghost_aabb[0], c_ghost_aabb[1]));
				
				exit->tag = C_PORTAL;
			
			director->exit_created = 1;
		}

		return;
	}
	
	for (int i = 0; i < director->pool.length; i++) {
		if (!pool_is_alloc(&director->pool, i))
			continue;
		
		ghost = pool_get(&director->pool, i);

		if (ghost->type == C_GHOST_REAPER) {
			c_ghost_reaper_update(director, player, ghost, t + ghost->t);
			
			continue;
		}

		vec3_sub(player->p->pos, ghost->entity->pos, p);
		vec3_sub(ghost->pos, ghost->entity->pos, p2);
		
		p[1] = 0;
		p2[1] = 0;
		
		float distance = vec3_length(p);
		float distance2 = vec3_length(p2);

		vec3_normalize(p, p);

		ghost->spr->u = (t % 60) > 30;
		
		float speed = c_ghost_attrib[ghost->type].speed * (rand() % 10) / 10.0;

		if (distance < c_ghost_attrib[ghost->type].shoot_dist)
			c_ghost_attrib[ghost->type].shoot(ghost, director, player, t + ghost->t);

		if (distance < 10.0 && distance2 < 3.0f) {
			if (distance < c_ghost_attrib[ghost->type].dist) {
				vec3_t v = { -p[2] * 3, 0, p[0] * 3 };

				if ((t + ((rand() % 10) - 5)) % 20 < 10)
					vec3_mulf(v, -1, v);
				
				vec3_mulf(p, (rand() % 2) * 2 - 1.3, p);
				vec3_add(v, p, v);
				
				c_phys_accelerate(ghost->pm, v, speed, 3.0f);
			} else {
				c_phys_accelerate(ghost->pm, p, speed, 6.0f);
			}
		} else {
			vec3_normalize(p2, p2);

			c_phys_accelerate(ghost->pm, p2, speed, 6.0f);
		}

		if (director->health->hp[g_scene_entity_id(director->scene, ghost->entity)] <= 0
			|| ghost->entity->pos[1] < -10.0f) {
			g_scene_remove_entity(director->scene, ghost->entity);
		}
	}
}

void g_director_remove(gdirector_t* director, gentity_t* entity) {
	cghost_t* g;
	
	for (int i = 0; i < director->pool.length; i++) {
		if (pool_is_alloc(&director->pool, i)) {
			g = pool_get(&director->pool, i);

			if (g->entity == entity) {
				pool_remove(&director->pool, g - (cghost_t*) director->pool.blk);

				director->enemy_count--;

				if (c_ghost_reaper_minion_count > 0)
					c_ghost_reaper_minion_count--;

				return;
			}
		}
	}
}

void g_director_set_exit(gdirector_t* director, vec3_t pos) {
	vec3_copy(director->exit, pos);
}