#ifndef M_POOL_H
#define M_POOL_H

#include <stdlib.h>

#define kb(x) x * 1024
#define mb(x) x * 1024 * 1024

typedef struct {
	int*	jmp;
	char*	blk;
	
	int		szblk;
	
	int		ptr;
	int		size;
	int		length;
} pool_t;

void		pool_init(pool_t* pool, void* blk, int* jmp, int size, int szblk);

void*		pool_alloc(pool_t* pool);

void*		pool_get(pool_t* pool, int i);

void		pool_remove(pool_t* pool, int i);

int			pool_is_alloc(pool_t* pool, int i);

#endif

#ifndef M_HUNK_H
#define M_HUNK_H

typedef struct {
	char*	ptr;
	char*	end;
	char*	block;
	
	int		size;
	
	int		used;
} memhunk_t;

void		hunk_init(memhunk_t* hunk, int size);

void*		hunk_alloc(memhunk_t* hunk, int size);

void*		hunk_ptr(memhunk_t* hunk);

void		hunk_reset(memhunk_t* hunk, void* reset);

void		hunk_pool_alloc(memhunk_t* hunk, pool_t* pool, int size, int szblk);

#endif

#ifndef M_FLOAT_BUFFER_T
#define M_FLOAT_BUFFER_T

typedef struct {
	void*	b;
	
	int		ptr;

	int		size;
	int		szblk;
} sbuf_t;

void		sbuf_init(sbuf_t* v, int size, int szblk);

void*		sbuf_alloc(sbuf_t* v, int size);

void*		sbuf_get(sbuf_t* v, int i);

void		sbuf_free(sbuf_t* v);

#endif