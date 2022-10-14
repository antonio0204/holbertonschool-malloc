#include "malloc.h"
#include "free_list.c"
#include <stdio.h>
#include <unistd.h>


/**
 * newFreeBlock - generate free block
 * @algnd_pyld_sz: size of memory requested
 * Return: pointer
 */
block_t *newFreeBlock(size_t algnd_pyld_sz)
{
	block_t *new_blk;
	size_t new_blk_sz;
	long page_sz;

	page_sz = sysconf(_SC_PAGESIZE);
	if (page_sz == -1)
	{
		fprintf(stderr, "newFreeBlock: sysconf failure\n");
		return (NULL);
	}

	for (new_blk_sz = (size_t)page_sz;
	     new_blk_sz < BLK_SZ(algnd_pyld_sz) + sizeof(block_t);
	     new_blk_sz += page_sz)
	{}

	new_blk = sbrk(new_blk_sz);
	if (new_blk == (void *)-1)
	{
		perror("newFreeBlock: sbrk");
		return (NULL);
	}

	new_blk->size = new_blk_sz;
	freeListAdd(new_blk);

	return (new_blk);
}

/**
 * printFreeList - test print
 *
 * @prefix: label string
 */
void printFreeList(char *prefix)
{
	void *pgm_brk;
	block_t *blk;
	int i;

	pgm_brk = sbrk(0);
	if (pgm_brk == (void *)-1)
	{
		perror("printFreeList: sbrk");
		return;
	}

	printf("[%s] program break: %10p\n", prefix, pgm_brk);
	printf("[%s] free list:\n", prefix);
	for (blk = first_free_blk, i = 0; blk; blk = blk->next, i++)
		printf("\t(%i) @%10p size:%lu next:%10p prev:%10p\n",
		       i, (void *)blk, blk->size,
		       (void *)(blk->next), (void *)(blk->prev));
}

/**
 * _malloc - my malloc
 * @size: size  in bytes
 * Return: pointer
 */
void *_malloc(size_t size)
{
	block_t *blk, *remainder_blk;
	size_t algnd_pyld_sz;

	algnd_pyld_sz = size + (ALIGN - (size % ALIGN));

	for (blk = first_free_blk; blk; blk = blk->next)
	{
		if (blk->size >= BLK_SZ(algnd_pyld_sz))
			break;
	}

	if (!blk)
		blk = newFreeBlock(algnd_pyld_sz);
	if (!blk)
		return (NULL);

	freeListRemove(blk);

	if (blk->size == BLK_SZ(algnd_pyld_sz))
		return (BLK_PAYLOAD(blk));

	remainder_blk = splitFreeBlock(blk, BLK_SZ(algnd_pyld_sz));
	if (!remainder_blk)
		return (NULL);

	freeListAdd(remainder_blk);

	return (BLK_PAYLOAD(blk));
}
