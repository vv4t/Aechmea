#include "m_local.h"

void sbuf_init(sbuf_t* sbuf, int szblk, int size) {
	sbuf->b = malloc(size * szblk);
	
	sbuf->szblk = szblk;

	sbuf->size = size;

	sbuf->ptr = 0;
}

void* sbuf_alloc(sbuf_t* sbuf, int size) {
	int ptr = sbuf->ptr;
	
	sbuf->ptr += size;
	
	if (sbuf->ptr > sbuf->size)
		sbuf->b = realloc(sbuf->b, (sbuf->size *= 2) * sbuf->szblk);

	return &((char*) sbuf->b)[ptr * sbuf->szblk];
}

void* sbuf_get(sbuf_t* sbuf, int i) {
	return &((char*) sbuf->b)[i * sbuf->szblk];
}

void sbuf_free(sbuf_t* sbuf) {
	free(sbuf->b);
}