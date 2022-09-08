#include "r_local.h"

r_texture_t	ptr_texture	= 0;
GLuint		pool_texture[32];

r_texture_t r_add_texture(void* pixels, int w, int h) {
	glGenTextures(1, &pool_texture[ptr_texture]);
	glBindTexture(GL_TEXTURE_2D, pool_texture[ptr_texture]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return ptr_texture++;
}

void r_bind_texture(r_texture_t texture, int i) {
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, pool_texture[texture]);
}
