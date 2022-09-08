#include "m_local.h"

void hunk_init(memhunk_t* hunk, int size) {
	hunk->used = 0;
	hunk->size = size;

	hunk->block	= malloc(size);
	hunk->ptr	= hunk->block;
	hunk->end	= hunk->block + size;
}

void* hunk_ptr(memhunk_t* hunk) {
	return hunk->ptr;
}

void hunk_reset(memhunk_t* hunk, void* pointer) {
	char* ptr = (char*) pointer;

	if (ptr < hunk->block || ptr > hunk->end)
		com_printf(LOG_ERROR, "Hunk reset out of bounds.");
	
	hunk->ptr = ptr;
	
	hunk->used = ptr - hunk->block;
}

void* hunk_alloc(memhunk_t* hunk, int size) {
	void* ptr = (void*) hunk->ptr;

	hunk->used += size;

	if ((hunk->ptr += size) > hunk->end)
		com_printf(LOG_ERROR, "Could not allocate to hunk. %i / %i", hunk->used, hunk->size);
	
	return ptr;
}

void hunk_pool_alloc(memhunk_t* hunk, pool_t* pool, int size, int szblk) {
	int*	jmp	= hunk_alloc(hunk, size * sizeof(int));
	char*	blk	= hunk_alloc(hunk, size * szblk);
	
	pool_init(pool, blk, jmp, size, szblk);
}