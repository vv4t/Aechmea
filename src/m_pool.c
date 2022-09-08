#include "m_local.h"

void pool_init(pool_t* pool, void* blk, int* jmp, int size, int szblk) {
	pool->blk	= blk;
	pool->jmp	= jmp;
	pool->size	= size;
	pool->szblk	= szblk;
	
	pool->ptr		= 0;
	pool->length	= 0;

	for (int i = 0; i < pool->size; i++)
		pool->jmp[i] = -1;
}

void* pool_alloc(pool_t* pool) {
	int ptr = pool->ptr;

	if (pool->jmp[ptr] != -1) {
		pool->ptr = pool->jmp[ptr];
		pool->jmp[ptr] = -1;

		if (pool->ptr > pool->length)
			pool->ptr = ++pool->length;
	} else {
		pool->length = ++pool->ptr;
		
		if (pool->length == pool->size)
			com_printf(LOG_ERROR, "pool: alloc fail %i/%i", pool->length, pool->size);
	}

	return &pool->blk[ptr * pool->szblk];
}

void pool_remove(pool_t* pool, int ptr) {
	pool->jmp[ptr] = pool->ptr;
	pool->ptr = ptr;

	if (pool->ptr == pool->length)
		pool->ptr = pool->length--;
}

int pool_is_alloc(pool_t* pool, int i) {
	return (i < pool->length && pool->jmp[i] == -1);
}

void* pool_get(pool_t* pool, int i) {
	return &pool->blk[i * pool->szblk];
}
