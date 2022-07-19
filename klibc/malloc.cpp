#include "klibc/malloc_internal.hpp"
#include "klibc/memory.hpp"
#include "klibc/mmap.hpp"

#include "kernel/kprint.hpp"

static MemoryMap s_MemoryMap;
static MallocBlockVector s_MallocAllocations(nullptr, 0);

MallocBlockVector::MallocBlockVector(void *baseAddress, size_t size)
{
	m_Block = reinterpret_cast<MallocNode*>(baseAddress);
	m_Size = size;
}

MallocBlockVector::~MallocBlockVector()
{
	if(m_Block)
	{
		//free(m_Block);
	}
}

MallocBlockVector::Iterator MallocBlockVector::begin()
{
	return m_Block;
}

MallocBlockVector::Iterator MallocBlockVector::end()
{
	return &m_Block[m_Count];
}

void MallocBlockVector::EmplaceBack(MallocNode element)
{
	this->Emplace(this->end(), element);
}

void MallocBlockVector::Emplace(
		MallocBlockVector::Iterator idx,
		MallocNode element)
{
	if (m_Size * sizeof(MallocNode) < m_Count + 1)
	{
		return; // TODO: realloc
	}

	for (size_t i = m_Count; &m_Block[i] >= idx; i--)
	{
		m_Block[i] = m_Block[i - 1];
	}
	m_Count++;
	*idx = element;
}

void initMalloc()
{
	fillMemoryMap(&s_MemoryMap);
}

//MallocLinkedList::ReverseIterator MallocLinkedList::rbegin()
//{
//}
//
//MallocLinkedList::ReverseIterator MallocLinkedList::rend()
//{
//}

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

const ueintptr_t base_free_mem_address = 0x200000;
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

