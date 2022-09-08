#include "r_local.h"

typedef GLuint	block_t;

r_block_t		ptr_block = 0;
block_t			pool_block[32];

r_block_t r_add_block(int size) {
	glGenBuffers(1, &pool_block[ptr_block]);

	glBindBuffer(GL_UNIFORM_BUFFER, pool_block[ptr_block]);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

	glBindBufferRange(GL_UNIFORM_BUFFER, ptr_block, pool_block[ptr_block], 0, size);

	return ptr_block++;
}

void r_bind_block(r_block_t block) {
	glBindBuffer(GL_UNIFORM_BUFFER, pool_block[block]);
}

void r_block_sub_data(void* data, int offset, int size) {
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void r_block_reset(r_block_t ptr) {
	for (int i = 0; i < ptr_block; i++) {
		glDeleteBuffers(1, &pool_block[i]);
	}

	ptr_block = 0;
}