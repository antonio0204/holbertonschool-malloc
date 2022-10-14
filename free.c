#include "malloc.h"

static block_t *first_free_blk;

/**
 * joinFreeBlocks - join free blocks
 */
void joinFreeBlocks(void)
{
	block_t *curr;
	void *pBrk;
	long pgSize;

	pBrk = sbrk(0);
	if (pBrk == (void *)-1)
	{
		perror("joinFreeBlocks: sbrk");
		return;
	}
	pgSize = sysconf(_SC_PAGESIZE);
	if (pgSize == -1)
	{
		perror("joinFreeBlocks: sysconf");
		return;
	}

	for (curr = first_free_blk; curr; curr = curr->next)
	{
		if ((uint8_t *)curr + curr->size == (uint8_t *)curr->next)
		{
			curr->size += curr->next->size;
			curr->next = curr->next->next;
			if (curr->next)
				curr->next->prev = curr;
		}
	}

	for (curr = first_free_blk; curr; curr = curr->next)
	{
		if ((uint8_t *)curr + curr->size == pBrk &&
		    curr->size >= (size_t)pgSize)
		{
			freeListRemove(curr);
			if (brk(curr) != 0)
				perror("joinFreeBlocks: brk");
			break;
		}
	}
}

/**
 * _free - own free
 * @ptr: pointer
 */
void _free(void *ptr)
{
	if (!ptr)
		return;

	freeListAdd(BLK_HEADER(ptr));
	joinFreeBlocks();
}
