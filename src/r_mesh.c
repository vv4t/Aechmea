#include "r_local.h"

typedef struct {
	GLuint		id;

	int			ptr;
	int			size;
} vbuf_t;

typedef struct {
	GLuint		vao;

	vbuf_t		index;
	vbuf_t		vertex;
} vframe_t;

typedef struct {
	int			vbo;
	int			ibo;

	int			szvbo;
	int			szibo;
} mesh_t;

r_mesh_t		ptr_mesh = 0;
mesh_t			pool_mesh[32];

static			vframe_t vram;

void vbuf_init(vbuf_t* vbuf, GLuint type, int size) {
	vbuf->ptr	= 0;
	vbuf->size	= size;

	glGenBuffers(1, &vbuf->id);
	glBindBuffer(type, vbuf->id);
	glBufferData(type, vbuf->size, 0, GL_DYNAMIC_DRAW);
}

void vframe_init(vframe_t* vframe, int VERTEX_BUFFER_SIZE, int INDEX_BUFFER_SIZE) {
	glGenVertexArrays(1, &vframe->vao);
	glBindVertexArray(vframe->vao);

	com_printf(LOG_DEBUG, "video: alloc buffer %i | %i", VERTEX_BUFFER_SIZE, INDEX_BUFFER_SIZE);

	vbuf_init(&vframe->vertex, GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE);
	vbuf_init(&vframe->index, GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_SIZE);
	
	int offset = 0;
	int stride = VERTEX_SIZE;
	
#ifdef VERTEX_ATTRIB_0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTEX_ATTRIB_0, GL_FLOAT, GL_FALSE, stride, 0);
	
	offset = offset + VERTEX_ATTRIB_0;
#endif
	
#ifdef VERTEX_ATTRIB_1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, VERTEX_ATTRIB_1, GL_FLOAT, GL_FALSE, stride, (float*) 0 + offset);
	
	offset = offset + VERTEX_ATTRIB_1;
#endif
	
#ifdef VERTEX_ATTRIB_2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, VERTEX_ATTRIB_2, GL_FLOAT, GL_FALSE, stride, (float*) 0 + offset);
	
	offset = offset + VERTEX_ATTRIB_2;
#endif

#ifdef VERTEX_ATTRIB_3
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, VERTEX_ATTRIB_3, GL_FLOAT, GL_FALSE, stride, (float*) 0 + offset);
#endif
	
}

void r_init_buffer(int VERTEX_BUFFER_SIZE, int INDEX_BUFFER_SIZE) {
	vframe_init(&vram, VERTEX_BUFFER_SIZE, INDEX_BUFFER_SIZE);
}

r_mesh_t r_alloc_mesh(int vbo, int ibo) {
	mesh_t* mesh = &pool_mesh[ptr_mesh];
	
	mesh->szvbo	= vbo;
	mesh->vbo	= vram.vertex.ptr;
	
	vram.vertex.ptr += vbo;

	com_printf(LOG_DEBUG, "video: vertex alloc %i/%i", vram.vertex.ptr, vram.vertex.size);
	
	if (vram.vertex.ptr * VERTEX_SIZE >= vram.vertex.size)
		com_printf(LOG_ERROR, "video: vertex alloc failed %i/%i", vram.vertex.ptr * VERTEX_SIZE, vram.vertex.size);
	
	if (ibo) {
		mesh->ibo = vram.index.ptr;
		mesh->szibo = ibo;

		vram.index.ptr += ibo;

		if (vram.index.ptr * sizeof(int) >= vram.index.size)
			com_printf(LOG_ERROR, "video: index alloc failed %i/%i", vram.index.ptr, vram.index.size);
		
		com_printf(LOG_DEBUG, "video: index alloc %i/%i", vram.index.ptr, vram.index.size);
	} else {
		mesh->ibo = 0;
		mesh->szibo = 0;
	}

	return ptr_mesh++;
}

void r_mesh_sub_vertex(r_mesh_t r_mesh, float* vertices, int offset, int size) {
	mesh_t* mesh = &pool_mesh[r_mesh];

	glBufferSubData(GL_ARRAY_BUFFER, (mesh->vbo + offset) * VERTEX_SIZE, size * VERTEX_SIZE, vertices);
}

void r_mesh_sub_index(r_mesh_t r_mesh, int* indices, int offset, int size) {
	mesh_t* mesh = &pool_mesh[r_mesh];

	for (int i = 0; i < size; i++)
		indices[i] += mesh->vbo;
	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (mesh->ibo + offset) * sizeof(int), size * sizeof(int), indices);
}

r_mesh_t r_add_mesh(float* vertices, int vbsize, int* indices, int ibsize) {
	r_mesh_t mesh;

	mesh = r_alloc_mesh(vbsize, ibsize);
	
	if (indices)
		r_mesh_sub_index(mesh, indices, 0, ibsize);
	
	r_mesh_sub_vertex(mesh, vertices, 0, vbsize);

	return mesh;
}

void r_draw_mesh(r_mesh_t r_mesh, int offset, int size) {
	mesh_t* mesh = &pool_mesh[r_mesh];

	if (!size)
		return;

	if (mesh->szibo) {
		if (offset + size > mesh->szibo)
			com_printf(LOG_ERROR, "video: cannot mesh draw %i > %i", offset + size, mesh->szibo);

		glDrawElements( GL_TRIANGLES, size, GL_UNSIGNED_INT, (int*) 0 + mesh->ibo + offset );
	} else {
		if (offset + size > mesh->szvbo)
			com_printf(LOG_ERROR, "video: cannot mesh draw %i > %i", offset + size, mesh->szvbo);

		glDrawArrays( GL_TRIANGLES, mesh->vbo + offset, size );
	}
}

void r_mesh_reset(r_mesh_t ptr) {
	mesh_t* mesh = &pool_mesh[ptr];

	vram.index.ptr = mesh->ibo;
	vram.vertex.ptr = mesh->vbo;

	ptr_mesh = 0;

	com_printf(LOG_DEBUG, "video: reset %i/%i", vram.vertex.ptr, vram.index.ptr);
}

int r_mesh_size(r_mesh_t r_mesh) {
	mesh_t* mesh = &pool_mesh[r_mesh];
	
	return mesh->szibo ? mesh->szibo : mesh->szvbo;
}
