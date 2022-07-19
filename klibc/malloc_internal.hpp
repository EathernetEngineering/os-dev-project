#ifndef _KLIBC_MALLOC_INTERNAL_HPP
#define _KLIBC_MALLOC_INTERNAL_HPP

#include <stddef.h>

struct MallocInfo
{
};

struct MallocNode
{
	size_t blockLength;
	void *block;
};

class MallocBlockVector final
{
	public:
		typedef MallocNode *Iterator;
//		typedef MallocNode *ReverseIterator;
	public:
		MallocBlockVector(void *baseAddress, size_t size);
		~MallocBlockVector();

		Iterator begin();
		Iterator end();
//		ReverseIterator rbegin();
//		ReverseIterator rend();

		void EmplaceBack(MallocNode element);
		void Emplace(Iterator idx, MallocNode element);


	private:
		MallocNode* m_Block;
		size_t m_Count;
		size_t m_Size;
};

void initMalloc();

#endif // _MALLOC_INTERNAL_HPP

