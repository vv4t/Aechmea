#include "c_local.h"

typedef enum {
	C_MAP_LIGHT_WALL = 1,
	C_MAP_LIGHT_FLOOR,
	C_MAP_GHOST_MAGE,
	C_MAP_GHOST_ORB,
	C_MAP_GHOST_WARRIOR,
	C_MAP_GHOST_NECROMANCER,
	C_MAP_GHOST_REAPER,
	C_MAP_HEALTH,
	C_MAP_EXIT,
	C_MAP_WALL		= 0b00010000,
	C_MAP_FLOOR		= 0b00100000,
	C_MAP_SOLID		= 0b01000000,
	C_MAP_OPAQUE	= 0b10000000,
} cmap_block_t;

void c_map_push_wall(sbuf_t* vertex, vec3_t p, vec3_t n, vec3_t t, int uv_x, int uv_y) {
	vec3_t b;

	float *v, *vn, *vt;

	float xc, yc, xx, yy;

	vec3_t j, k;

	vec3_cross(n, t, b);

	float px = 1.0 / PX_SPRITE_W / 64.0;
	float py = 1.0 / PX_SPRITE_H / 64.0;

	for (int i = 0; i < 4; i++) {
		v = sbuf_alloc(vertex, 3);
		vn = sbuf_alloc(vertex, 3);
		vt = sbuf_alloc(vertex, 2);

		xc = i % 2;
		yc = i / 2;

		xx = xc - 0.5f;
		yy = yc - 0.5f;

		vec3_mulf(t, xx, j);
		vec3_mulf(b, yy, k);

		vec3_add(p, j, v);
		vec3_add(v, k, v);

		vec3_init(vn);

		vt[0] = xc * (PX_SPRITE_W - 2 * px) + px + uv_x * PX_SPRITE_W;
		vt[1] = (1 - yc) * (PX_SPRITE_H - 2 * py) + py + uv_y * PX_SPRITE_H;
	}
}

cmap_block_t c_map_blk_at(cmap_t* map, int x, int y) {
	if (x < 0 || y < 0 || x >= map->w || y >= map->h)
		return 0;
	
	return map->m[x + y * map->w];
}

void c_map_load(cmap_t* map, gscene_t* scene, grender_t* render, gdirector_t* director, gphys_t* phys) {
	sbuf_t index, vertex;

	vec3_t v;
	vec3_t pos;
	vec3_t normal;
	vec3_t tangent;

	vec4_t color = { 1, 0, 0, 3.0f };
	vec4_t portal_color = { 1, 0, 0, 1.0f };
	vec4_t crystal_color = { 0, 1, 1, 2.0f };

	int uv[2];

	int q = 0;

	sbuf_init(&index, sizeof(int), 32);
	sbuf_init(&vertex, sizeof(float), 32);
	
	vec3_set(normal, 0.0f, 1.0f, 0.0f);
	vec3_set(tangent, 1.0f, 0.0f, 0.0f);

	cmap_block_t blk;
	
	for (int y = 0; y < map->h; y++) {
		for (int x = 0; x < map->w; x++) {
			pos[0] = x + 0.5f;
			pos[1] = 0;
			pos[2] = y + 0.5f;

			blk = c_map_blk_at(map, x, y);

			if (blk & C_MAP_WALL) {
				uv[0] = 1;
				uv[1] = 1;
			}
			
			if (blk & C_MAP_FLOOR) {
				uv[0] = 0;
				uv[1] = 1;
			}

			switch (blk & 0b1111) {
				case C_MAP_LIGHT_WALL:
					uv[0] = 2;
					uv[1] = 1;

					break;

				case C_MAP_LIGHT_FLOOR:
					uv[0] = 0;
					uv[1] = 2;

					g_render_light_add(render, pos, color);

					break;
				
				case C_MAP_HEALTH:
					g_render_light_add(render, pos, crystal_color);
					g_director_add_health(director, pos);

					break;
				
				case C_MAP_EXIT:
					uv[0] = 0;
					uv[1] = 2;

					g_render_light_add(render, pos, portal_color);
					g_director_set_exit(director, pos);

					break;
				
				case C_MAP_GHOST_ORB:
				case C_MAP_GHOST_MAGE:
				case C_MAP_GHOST_WARRIOR:
				case C_MAP_GHOST_NECROMANCER:
				case C_MAP_GHOST_REAPER:
					g_director_add_ghost(director, (blk & 0b1111) - C_MAP_GHOST_MAGE, pos);

					break;
			}

			if (blk & C_MAP_WALL) {
				for (int i = 0; i < 4; i++) {
					normal[(i + 1) % 2 * 2] = 0;
					normal[(i % 2) * 2] = (i / 2) * 2 - 1;

					if (c_map_blk_at(map, x + normal[0], y + normal[2]) & C_MAP_FLOOR) {
						vec3_mulf(normal, 0.5f, v);

						vec3_add(v, pos, v);

						vec3_set(tangent, 0.0f, 1.0f, 0.0f);

						c_map_push_wall(&vertex, v, normal, tangent, uv[0], uv[1]);

						if ((blk & 0b1111) == C_MAP_LIGHT_WALL) {
							vec3_mulf(normal, 1.0f, v);
							vec3_add(v, pos, v);

							v[1] = 1;

							g_render_light_add(render, v, color);
						}

						q++;
					}
				}

				vec3_set(normal, 0.0f, 1.0f, 0.0f);

				pos[1] = 1;
			}

			if (blk & C_MAP_FLOOR) {
				q++;

				if (rand() % 2 == 0)
					vec3_set(tangent, 1.0f, 0.0f, 0.0f);
				else 
					vec3_set(tangent, 0.0f, 0.0f, 1.0f);

				c_map_push_wall(&vertex, pos, normal, tangent, uv[0], uv[1]);
			}
		}
	}

	int* id;

	for (int i = 0; i < q; i++) {
		int a = i * 4;

		id = sbuf_alloc(&index, 6);
		
		for (int j = 0; j < 6; j++) {
			int b = j % 3 + j / 3;

			id[j] = a + b;
		}
	}

	r_mesh_t mesh = r_add_mesh(vertex.b, q * 4, index.b, q * 6);
	
	sbuf_free(&index);
	sbuf_free(&vertex);

	vec3_set(v, 0, 0, 0);

	g_render_add(render, map->entity, mesh);
	g_phys_add_collider(phys, c_phys_map_init(v, map->m, map->w, map->h, C_MAP_SOLID));
	g_phys_add_collider(phys, c_phys_ground_init(map->m, C_MAP_FLOOR, map->w, map->h, 0.0f));
}

void c_map_init(cmap_t* map, gscene_t* scene, grender_t* render, gdirector_t* director, gphys_t* phys,
					unsigned char* pixels, int w, int h) {
	int col;

	int r, g, b;
	
	map->m = hunk_alloc(&scene->hunk, w * h);

	map->w = w;
	map->h = h;

	for (int i = 0; i < w * h; i++) {
		r = pixels[i * 4 + 0];
		g = pixels[i * 4 + 1];
		b = pixels[i * 4 + 2];
		
		col = r << 16 | g << 8 | b;

		switch (col) {
			case 0xffffff:
				map->m[i] = C_MAP_FLOOR;
				break;
			
			case 0xff0000:
				map->m[i] = C_MAP_WALL | C_MAP_SOLID;
				break;
			
			case 0xffff00:
				map->m[i] = C_MAP_LIGHT_WALL | C_MAP_WALL | C_MAP_SOLID;
				break;
			
			case 0xffcc00:
				map->m[i] = C_MAP_LIGHT_FLOOR | C_MAP_FLOOR;
				break;
			
			case 0xff5b00:
				map->m[i] = C_MAP_GHOST_ORB | C_MAP_FLOOR;
				break;
			
			case 0xff00ff:
				map->m[i] = C_MAP_GHOST_MAGE | C_MAP_FLOOR;
				break;
			
			case 0xa7a7a7:
				map->m[i] = C_MAP_GHOST_WARRIOR | C_MAP_FLOOR;
				break;
			
			case 0x494949:
				map->m[i] = C_MAP_GHOST_NECROMANCER | C_MAP_FLOOR;
				break;
			
			case 0x0000ff:
				map->m[i] = C_MAP_GHOST_REAPER;
				break;
			
			case 0x00ffff:
				map->m[i] = C_MAP_HEALTH | C_MAP_FLOOR;
				break;
			
			case 0x00ff00:
				map->m[i] = C_MAP_EXIT | C_MAP_FLOOR;
				break;
			
			default:
				map->m[i] = 0;
				break;
		}
	}

	vec3_t v = {0, 0, 0};

	map->entity = g_scene_add_entity(scene);
		map->entity->tag = C_MAP;

		c_map_load(map, scene, render, director, phys);
}