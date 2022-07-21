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
		typedef const MallocNode *ConstIterator;
//		typedef MallocNode *ReverseIterator;
	public:
		MallocBlockVector(void *baseAddress, size_t size);
		MallocBlockVector();
		~MallocBlockVector();

		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;
//		ReverseIterator rbegin();
//		ReverseIterator rend();

		size_t GetCount() const { return m_Count; }
		size_t GetCapacity() const { return m_Size * sizeof(MallocNode); }
		size_t GetSize() const { return m_Size; }

		void Insert(MallocNode node);
		void RemoveBack();
		void Remove(Iterator idx);

		void Sort();
		Iterator Find(void *addr);

	private:
		void EmplaceBack(MallocNode element);
		void Emplace(Iterator idx, MallocNode element);

	private:
		static int64_t s_Instances;

	private:
		MallocNode* m_Block;
		size_t m_Size;
		size_t m_Count;
};

void initMalloc();

void *malloc(size_t size); // throw();
void *realloc(void *addr, size_t size); //throw();
void free(void *addr) noexcept;

#endif // _MALLOC_INTERNAL_HPP

