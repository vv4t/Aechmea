#include "asset.h"

#include "common.h"

#include "lodepng.h"

void asset_init(asset_t* asset, int mem) {
	hunk_init(&asset->stack, mem);

	asset->list_tex = NULL;
	asset->list_mesh = NULL;
	asset->list_file = NULL;
}

asset_tex_t* asset_find_texture(asset_t* asset, const char* path) {
	asset_tex_t* it = asset->list_tex;

	while (it != NULL) {
		if (strcmp(it->key, path) == 0)
			return it;

		it = it->next;
	}

	return NULL;
}

asset_file_t* asset_find_file(asset_t* asset, const char* path) {
	asset_file_t* it = asset->list_file;

	while (it != NULL) {
		if (strcmp(it->key, path) == 0)
			return it;

		it = it->next;
	}

	return NULL;
}

asset_mesh_t* asset_find_mesh(asset_t* asset, const char* path) {
	asset_mesh_t* it = asset->list_mesh;

	do {
		it = it->next;

		if (strcmp(it->key, path) == 0)
			return it; 
	} while (it != asset->list_mesh);

	return NULL;
}

asset_tex_t* asset_load_texture(asset_t* asset, const char* path) {
	unsigned char* data;
	unsigned int width, height;
	
	if (lodepng_decode32_file(&data, &width, &height, path)) {
		com_printf(LOG_ERROR, "Failed to open %s\n", path);
		
		return NULL;
	}
	
	int size = width * height * sizeof(int);

	asset_tex_t* tex = hunk_alloc(&asset->stack, sizeof(asset_tex_t));
		tex->pixels = hunk_alloc(&asset->stack, size);
		tex->w		= width;
		tex->h		= height;
		tex->key	= path;
	
	if (!asset->list_tex) {
		tex->next = NULL;
		asset->list_tex = tex;
	} else {
		tex->next = asset->list_tex;
		asset->list_tex = tex;
	}

	memcpy(tex->pixels, data, size);
	
	free(data);
	
	return tex;
}

asset_file_t* asset_load_file(asset_t* asset, const char* path) {
	FILE* fs = fopen(path, "rb");

	if (!fs)
		com_printf(LOG_ERROR, "asset: failed to open file %s", path);
	
	fseek(fs, 0L, SEEK_END);
	long length = ftell(fs);

	fseek(fs, 0L, SEEK_SET);

	asset_file_t* file = hunk_alloc(&asset->stack, sizeof(asset_file_t));
		file->length = length;
		file->buffer = hunk_alloc(&asset->stack, length + 1);
		file->key = path;


	fread(file->buffer, 1, length, fs);
	fclose(fs);

	file->buffer[length] = '\0';

	if (!asset->list_file) {
		file->next = NULL;
		asset->list_file = file;
	} else {
		file->next = asset->list_file;
		asset->list_file = file;
	}
	
	return file;
}


asset_mesh_t* asset_load_mesh(asset_t* asset, const char* path) {
	FILE* file = fopen(path, "rb");
	
	if (!file) {
		com_printf(LOG_ERROR, "failed to load file: %s", path);
		
		return NULL;
	}
	
	char op[8], line[256];
	
	sbuf_t v, vn, vt, vbuf;
	
	sbuf_init(&v, sizeof(float), 32);
	sbuf_init(&vn, sizeof(float), 32);
	sbuf_init(&vt, sizeof(float), 32);
	
	sbuf_init(&vbuf, sizeof(float), 32);
	
	float* uv, *pos, *normal, *vertex;
	
	int f_v[3];
	int f_vn[3];
	int f_vt[3];
	
	while (fgets(line, 256, file)) {
		sscanf(line, "%s", op);
		
		if ( strcmp(op, "v") == 0) {
			pos = sbuf_alloc(&v, 3);
			
			sscanf(line, "v %f %f %f", &pos[0], &pos[1], &pos[2]);
		}
		
		if ( strcmp(op, "vn") == 0) {
			normal = sbuf_alloc(&vn, 3);
			
			sscanf(line, "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
		}
		
		if ( strcmp(op, "vt") == 0) {
			uv = sbuf_alloc(&vt, 2);
			
			sscanf(line, "vt %f %f", &uv[0], &uv[1]);

			uv[1] = 1 - uv[1];
		}
		
		if ( strcmp(op, "f") == 0) {
			sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i",
				&f_v[0], &f_vt[0], &f_vn[0],
				&f_v[1], &f_vt[1], &f_vn[1],
				&f_v[2], &f_vt[2], &f_vn[2]
			);
			
			for (int i = 0; i < 3; i++) {
				vertex = sbuf_alloc(&vbuf, 8);
				
				f_v[i]	= (f_v[i] - 1) * 3;
				f_vn[i]	= (f_vn[i] - 1) * 3;
				f_vt[i]	= (f_vt[i] - 1) * 2;
				
				memcpy(vertex, sbuf_get(&v, f_v[i]), 3 * sizeof(float));
				memcpy(vertex + 3, sbuf_get(&vn, f_vn[i]), 3 * sizeof(float));
				memcpy(vertex + 6, sbuf_get(&vt, f_vt[i]), 2 * sizeof(float));
			}
		}
	}
	
	asset_mesh_t* mesh = hunk_alloc(&asset->stack, sizeof(asset_mesh_t));
		mesh->size = vbuf.ptr / 8;
		mesh->vertices = hunk_alloc(&asset->stack, vbuf.ptr * sizeof(float));
		mesh->key = path;

	memcpy(mesh->vertices, vbuf.b, vbuf.ptr * sizeof(float));
	
	if (!asset->list_mesh)
		asset->list_mesh = mesh;
	
	mesh->next = asset->list_mesh;
	
	asset->list_mesh->next = mesh;
	asset->list_mesh = mesh;
	
	sbuf_free(&v);
	sbuf_free(&vn);
	sbuf_free(&vt);
	sbuf_free(&vbuf);
	
	return mesh;
}