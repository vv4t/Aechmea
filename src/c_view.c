#include "c_local.h"

void c_view_init(cview_t* view) {
	vec3_init(view->pos);
	quat_init(view->rot);
}

void c_view_free_move(cview_t* view, cinput_t* input) {
	vec3_t v;

	vec3_rotate(input->axis, view->rot, v);

	vec3_add(view->pos, v, view->pos);
}

void c_view_free_look(cview_t* view, cinput_t* input) {
	vec3_t v;
	quat_t q;

	vec3_set(v, 0, 1, 0);
	quat_rotate(q, v, 0.005 * input->yaw);
	
	quat_mul(q, view->rot, view->rot);
	
	vec3_set(v, 1, 0, 0);
	vec3_rotate(v, view->rot,  v);
	quat_rotate(q, v, 0.005 * input->pitch);
	
	quat_mul(q, view->rot, q);
	quat_normalize(q, view->rot);
}

void c_view_update(cview_t* view) {
	mat4_t m;
	vec3_t v;
	quat_t q;
	
	mat4_copy(view->p, view->m);
	
	quat_conjugate(view->rot, q);
	
	mat4_rotate(m, q);
	mat4_mul(view->m, m, view->m);
	
	vec3_mulf(view->pos, -1, v);
	
	mat4_translate(m, v);
	mat4_mul(view->m, m, view->m);
}
