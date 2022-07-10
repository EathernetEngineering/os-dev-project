#ifndef _MALLOC_INTERNAL_HPP
#define _MALLOC_INTERNAL_HPP

#include <stddef.h>

struct MallocInfo
{
};

class MallocLinkedListIterator
{

};

#define MALLOC_LINKED_LIST_NODE_HEAD_MAGIC_NUMBER (void*)(0xABCDEF)
#define MALLOC_LINKED_LIST_NODE_TAIL_MAGIC_NUMBER (void*)(0xABCDEF)
#define MALLOC_BASE_ADDRESS (void*)(0x200000);
#define MALLOC_MAXIMUM_ADDRESS (void*)((uintptr_t)MALLOC_BASE_ADDRESS + (uintptr_t)(0x200000));

struct MallocLinkedListNode
{
	struct MallocLinkedListNodeHeader
	{
		MallocLinkedListNode* next;
		MallocLinkedListNode* prev;
	} __attribute__((packed));

	MallocLinkedListNodeHeader header;
	size_t size;
	void *block;
} __attribute__((packed));

class MallocLinkedList final
{
	public:
		MallocLinkedList(MallocLinkedListNode *head);
		~MallocLinkedList() {}

	private:
		MallocLinkedListNode *m_Head;
};

#endif // _MALLOC_INTERNAL_HPP

