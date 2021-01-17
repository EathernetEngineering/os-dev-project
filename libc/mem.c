#include "mem.h"
#include "function.h"
#include "stddef.h"

void bcopy(const void *src, void *dest, unsigned long len)
{
	if (dest < src)
	{
		const char *firsts = (const char*)src;
		char *firstd = (char*)dest;
		while (len--)
			*firstd++ = *firsts++;
	}
	else
	{
		const char *lasts = (const char*)src + (len-1);
		char* lastd = (char*)dest + (len-1);
		while (len--)
			*lastd-- = *lasts--;
	}
}

void* memcpy(void* out, const void* in, unsigned long length)
{
	bcopy(in, out, length);
	return out;
}

void* memset(void* dest, register int val, register long len)
{
	register unsigned char *ptr = (unsigned char*)dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

#ifndef align_up
#define align_up(num, align) \
	(((num) + ((align) - 1)) & ~((align) - 1))
#endif

typedef struct ll_head {
	struct ll_head *next;
	struct ll_head *prev;
} ll_t;

typedef struct {
	ll_t node;
	size_t size;
	char *block;
} alloc_node_t;

/*
 * For now there is a hard limit of 1GiB of memory that can be allocated
 * on the heap. This will be changed in the future, when we can find out 
 * how much memory the system has.
 */
const uintptr_t base_free_mem_address = 0x10000;
const uintptr_t max_mem_address = base_free_mem_address + 0x40000000;
uintptr_t free_mem_address = 0x10000;

ll_t list = { (void *)base_free_mem_address, (void *)base_free_mem_address };

uintptr_t kmalloc(size_t size)
{
	uintptr_t ptr = NULL;
	alloc_node_t *blk = NULL;

	if (size > 0)
	{
		size = align_up(size, sizeof(void *));

		for (blk = (alloc_node_t *)container_of(list.next, __typeof__(*blk), node); &blk->node != (&list); blk = container_of(blk->node.next, __typeof__(*blk), node))
		{
			if (blk->size >= size)
			{
				ptr = (uintptr_t)&blk->block;
				break;
			}
		}

		if (ptr)
		{
			if ((blk->size - size) >= offsetof(alloc_node_t, block) + 4)
			{
				alloc_node_t *new_blk;
				new_blk = (alloc_node_t *)((uintptr_t)(&blk->block) + size);
				new_blk->size = blk->size - size - offsetof(alloc_node_t, block);
				blk->size = size;
				blk->node.next->prev = &new_blk->node;
				new_blk->node.next = blk->node.next;
				new_blk->node.prev = &blk->node;
				blk->node.next = &new_blk->node;
			}
			blk->node.next->prev = blk->node.next;
			blk->node.prev->next = blk->node.prev;
			blk->node.next = NULL;
			blk->node.prev = NULL;
		}
	}
	return ptr;
}

void *malloc_addblock(void *addr, size_t size)
{
	alloc_node_t *blk;

	blk = (void *) align_up((uintptr_t)addr, sizeof(void*));

	if ((uintptr_t)blk < base_free_mem_address || ((uintptr_t)blk + (uintptr_t)size) > max_mem_address) asm volatile("int $0x05");
	
	blk->size = (uintptr_t) addr + size - (uintptr_t) blk - offsetof(alloc_node_t, block);

	return blk->block;
}

