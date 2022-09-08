#ifndef RENDER_H
#define RENDER_H

#define R_DEPTH 0x0100
#define R_COLOR 0x4000

#define VERTEX_SIZE			32

typedef char	r_mesh_t;
typedef char	r_block_t;
typedef char	r_shader_t;
typedef char	r_texture_t;

void			r_init(int vertex_buffer_size, int index_buffer_size);

void			r_clear(int buffer);

void			r_color(float r, float g, float b);

#endif

#ifndef R_MESH_H
#define R_MESH_H


r_mesh_t		r_add_mesh(float* vertices, int vbsize, int* indices, int ibsize);

r_mesh_t		r_alloc_mesh(int vbo, int ibo);

void			r_mesh_sub_index(r_mesh_t mesh, int* indices, int offset, int ibsize);

void			r_mesh_sub_vertex(r_mesh_t mesh, float* vertices, int offset, int vbsize);

void			r_draw_mesh(r_mesh_t mesh, int offset, int size);

int				r_mesh_size(r_mesh_t mesh);

void			r_mesh_reset(r_mesh_t ptr);

#endif

#ifndef R_SHADER_H
#define R_SHADER_H

r_shader_t		r_add_shader(const char* vertex, const char* pixel);

void			r_bind_shader(r_shader_t shader);

void			r_uniform_block(r_shader_t shader, const char* name, r_block_t block);

#endif

#ifndef R_TEXTURE_H
#define R_TEXTURE_H

r_texture_t		r_add_texture(void* pixels, int w, int h);

void			r_bind_texture(r_texture_t texture, int sampler);

#endif

#ifndef R_BLOCK_H
#define R_BLOCK_H

r_block_t		r_add_block(int size);

void			r_bind_block(r_block_t block);

void			r_block_sub_data(void* data, int offset, int size);

void			r_block_reset(r_block_t ptr);

#endif
