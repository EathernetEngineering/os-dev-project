#include "libc/malloc_internal.hpp"
#include "libc/memory.hpp"

#include "kernel/kprint.hpp"

MallocLinkedList* MallocLinkedList::s_pInstance = nullptr;

MallocLinkedList::MallocLinkedList(MallocLinkedListNode *head)
{
	if (s_pInstance == nullptr)
	{
		m_Head = head;
		m_Head->header.next = reinterpret_cast<MallocLinkedListNode*>(MALLOC_LINKED_LIST_NODE_TAIL_MAGIC_NUMBER);
		m_Head->header.prev = reinterpret_cast<MallocLinkedListNode*>(MALLOC_LINKED_LIST_NODE_HEAD_MAGIC_NUMBER);
		m_Head->blockSize = 0;
		m_Head->block = MALLOC_BASE_ADDRESS;
	}
	else
	{
		// TODO: Handle errors better.
		kcrit("Only one instance of malloc linked list can exist!");
	}
}

/*
 * For now there is a hard limit of 1GiB of memory that can be allocated
 * on the heap. This will be changed in the future, when we can find out
 * how much memory the system has.
 *
#ifndef align_up
#define align_up(num, align) \
	((uint64_t)((num) + ((align) - 1)) & (uint64_t)~((align) - 1))
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

const uintptr_t base_free_mem_address = 0x200000;
const uintptr_t max_mem_address = base_free_mem_address + 0x200000;
uintptr_t free_mem_address = base_free_mem_address;

ll_t list = { (void *)base_free_mem_address, (void *)base_free_mem_address };

void *kmalloc(size_t size)
{
	void *ptr = NULL;
	alloc_node_t *blk = NULL;

	if (size > 0)
	{
		size = align_up(size, sizeof(void *));

		for (blk = (alloc_node_t *)container_of(list.next, __typeof__(*blk), node); &blk->node != (&list); blk = container_of(blk->node.next, __typeof__(*blk), node))
		{
			if (blk->size >= size)
			{
				ptr = (void *)&blk->block;
				break;
			}
		}

		if (ptr)
		{
			if ((blk->size - size) >= offsetof(alloc_node_t, block) + 4)
			{
				alloc_node_t *new_blk;
				new_blk = (alloc_node_t *)((&blk->block) + size);
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

	blk = (void *) align_up(addr, sizeof(void*));

	if ((uint64_t)blk < (uint64_t)base_free_mem_address ||
			(uint64_t)(blk + size) > (uint64_t)max_mem_address)
		asm volatile("int $0x05");
	
	blk->size = addr + size - (void*)blk - offsetof(alloc_node_t, block);

	return blk->block;
}
*/

