#include "c_local.h"

void g_gui_init(ggui_t* gui, int size) {
	gui->size = size;
	gui->ptr = 0;

	gui->mesh = r_alloc_mesh(size * 4, size * 6);
	
	int* index = malloc(size * 6 * sizeof(int));

	for (int i = 0; i < size; i++) {
		int a = i * 4;

		for (int j = 0; j < 6; j++)
			index[i * 6 + j] = a + j % 3 + j / 3;
	}

	r_mesh_sub_index(gui->mesh, index, 0, size * 6);

	free(index);
}

int g_gui_add_rect(ggui_t* gui, float x, float y, float w, float h,
						float u, float v, float tw, float th) {
	int ptr = gui->ptr++;

	g_gui_update(gui, ptr, x, y, w, h, u, v, tw, th);

	return ptr;
}

void g_gui_update(ggui_t* gui, int g, float x, float y, float w, float h,
						float u, float v, float tw, float th) {
	float vertex[32];

	float *vt;

	for (int yc = 0; yc < 2; yc++) {
		for (int xc = 0; xc < 2; xc++) {
			vt = &vertex[(xc + yc * 2) * 8];

			vt[0] = 2 * (x + xc * w) - 1;
			vt[1] = 2 * (y + yc * h) - 1;
			vt[2] = 0;

			vt[3] = 0; vt[4] = 0; vt[5] = 0;

			vt[6] = (u + xc * tw) * PX_SPRITE_W;
			vt[7] = (v + yc * th) * PX_SPRITE_H;
		}
	}
	
	r_mesh_sub_vertex(gui->mesh, vertex, g * 4, 4);
}

void g_gui_reset(ggui_t* gui, int ptr) {
	gui->ptr = ptr;
}

void g_gui_render(ggui_t* gui) {
	if (gui->ptr)
		r_draw_mesh(gui->mesh, 0, gui->ptr * 6);
}