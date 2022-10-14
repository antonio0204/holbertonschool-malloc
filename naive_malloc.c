#include "malloc.h"
#include <unistd.h>
#include <stdio.h>


#ifdef ALIGN
#undef ALIGN
#define ALIGN sizeof(void *)
#endif

/**
* putPageH - put page to Heap
* Return: pointer
*/
void *putPageH(void)
{
	long int pgSize;
	void *ptr;

	pgSize = sysconf(_SC_PAGESIZE);
	if (pgSize == -1)
	{
		fprintf(stderr, "putPageH: sysconf failure\n");
		return (NULL);
	}

	ptr = sbrk((intptr_t)pgSize);
	if (ptr == (void *)-1)
	{
		perror("putPageH: sbrk");
		return (NULL);
	}

	return (ptr);
}

/**
 * findFblock - finds free block
 * @addr0: address
 * @size: size
 * @usedBcount: counter
 * @freeBaddr: free block address
 * @freeBsize: size of free blocks
 * Return: 0
 */
int findFblock(void *addr0, size_t *size, size_t usedBcount,
		    void **freeBaddr, size_t *freeBsize)
{
	void *blockAddr;
	long int pgSize;
	size_t i, bkSize;

	if (addr0 == NULL ||  size == NULL ||
	    freeBaddr == NULL || freeBsize == NULL)
		return (1);

	pgSize = sysconf(_SC_PAGESIZE);
	if (pgSize == -1)
	{
		fprintf(stderr, "findFblock: sysconf failure\n");
		return (1);
	}

	for (blockAddr = addr0, i = 0; i < usedBcount; i++)
	{
		bkSize = *((size_t *)blockAddr);
		blockAddr = (unsigned char *)blockAddr + bkSize;
	}

	*freeBaddr = blockAddr;
	*freeBsize = usedBcount ?
		*((size_t *)*freeBaddr) : (size_t)pgSize;

	*size += (ALIGN - (*size % ALIGN));

	while (*freeBsize < (sizeof(size_t) * 2) + *size)
	{
		if (putPageH() == NULL)
			return (1);
		*freeBsize += pgSize;
		*((size_t *)*freeBaddr) = *freeBsize;
	}

	return (0);
}

/**
 * naive_malloc - malloc but naive
 * @size: the size to be
 * Return: pointer
 */
void *naive_malloc(size_t size)
{
	static void *addr0;
	static size_t usedBcount;
	void *brkptr, *pLaddr, *newBaddr, *freeBaddr;
	size_t nwBsize, freeBsize;

	if (!addr0)
	{
		addr0 = putPageH();
		if (addr0 == NULL)
			return (NULL);
	}

	brkptr = sbrk(0);
	if (brkptr == (void *)-1)
	{
		perror("naive_malloc: sbrk");
		return (NULL);
	}

	if (findFblock(addr0, &size, usedBcount,
			    &freeBaddr, &freeBsize) == 1)
		return (NULL);

	newBaddr = freeBaddr;
	nwBsize = sizeof(size_t) + size;
	*((size_t *)newBaddr) = nwBsize;
	usedBcount++;

	freeBsize -= nwBsize;
	freeBaddr = (unsigned char *)freeBaddr + nwBsize;
	*((size_t *)freeBaddr) = freeBsize;

	pLaddr = (unsigned char *)newBaddr + sizeof(size_t);
	return (pLaddr);
}
