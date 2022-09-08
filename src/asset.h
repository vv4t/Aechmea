#ifndef ASSET_H
#define ASSET_H

#include "memory.h"

typedef struct file_s {
	const char*	key;

	struct file_s* next;
	
	char* buffer;

	int length;
} asset_file_t;

typedef struct mesh_s {
	const char*	key;
	
	struct mesh_s* next;

	float*	vertices;
	
	int		size;
} asset_mesh_t;

typedef struct tex_s {
	const char*	key;

	struct tex_s* next;

	void*	pixels;
	
	int		w;
	int		h;
} asset_tex_t;

typedef struct {
	memhunk_t		stack;

	asset_tex_t*	list_tex;

	asset_mesh_t*	list_mesh;
	
	asset_file_t*	list_file;
} asset_t;

void				asset_init(asset_t* asset, int mem);

asset_mesh_t*		asset_load_mesh(asset_t* asset, const char* math);

asset_file_t*		asset_load_file(asset_t* asset, const char* path);

asset_tex_t*		asset_load_texture(asset_t* asset, const char* path);

asset_file_t*		asset_find_file(asset_t* asset, const char* path);

asset_mesh_t*		asset_find_mesh(asset_t* asset, const char* path);

asset_tex_t*		asset_find_texture(asset_t* asset, const char* path);

#endif
