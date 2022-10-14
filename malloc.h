#ifndef HOLBI_MALLOC
#define HOLBI_MALLOC

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define ALIGN (2 * sizeof(void *))
#define HEADER_SZ (sizeof(size_t) * 2)
#define BLK_SZ(size) (HEADER_SZ + size)
#define BLK_PAYLOAD(ptr) ((void *)((uint8_t *)ptr + HEADER_SZ))
#define BLK_HEADER(ptr) ((block_t *)((uint8_t *)ptr - HEADER_SZ))

/**
 * struct block_s - Block struct
 * @prev_size: prev size
 * @size: actuall size
 * @next: pointer
 * @prev: pointer
 */
typedef struct block_s
{
	size_t prev_size;
	size_t size;
	struct block_s *next;
	struct block_s *prev;

} block_t;

void *putPageH(void);
int findFblock(void *addr0, size_t *size, size_t usedBcount,
		    void **freeBaddr, size_t *freeBsize);
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void printFreeList(char *prefix);
block_t *newFreeBlock(size_t algnd_pyld_sz);

void freeListAdd(block_t *blk);
void freeListRemove(block_t *blk);
block_t *splitFreeBlock(block_t *free_blk, size_t size);
void _free(void *ptr);
void joinFreeBlocks(void);

#endif /* HOLBI_MALLOC */
