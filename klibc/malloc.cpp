#include "klibc/malloc_internal.hpp"
#include "klibc/memory.hpp"
#include "klibc/mmap.hpp"
#include "klibc/function.hpp"

#include "kernel/kprint.hpp"

static MemoryMap s_MemoryMap;
static MallocBlockVector s_MallocAllocations;

int64_t MallocBlockVector::s_Instances = 0;

MallocBlockVector::MallocBlockVector()
	: MallocBlockVector(reinterpret_cast<void *>(0x00), 0)
{
}

MallocBlockVector::MallocBlockVector(void *baseAddress, size_t size)
	: m_Block(reinterpret_cast<MallocNode*>(baseAddress)), m_Size(size),
	  m_Count()
{
	s_Instances++;

	if (!memoryInUsableRange(&s_MemoryMap, (uintptr_t)baseAddress, size))
		return; //TODO: think about throwing an exception

	if (m_Block != nullptr && m_Size > 0)
		this->EmplaceBack({ m_Size, m_Block });
}

MallocBlockVector::~MallocBlockVector()
{
	if (s_Instances <= 0)
	{
		if(m_Block)
		{
			free(m_Block);
		}
	}
	s_Instances--;
}

MallocBlockVector::Iterator MallocBlockVector::begin()
{
	return m_Block;
}

MallocBlockVector::Iterator MallocBlockVector::end()
{
	return &m_Block[m_Count];
}

void MallocBlockVector::Insert(MallocNode node)
{
	Iterator itr = this->end() - 1;
	if (node.block >  itr->block)
	{
		this->Emplace(itr + 1, node);
		return;
	}
	while (itr != this->begin())
	{
		if (node.block <= itr->block) break;
		--itr;
	}
	this->Emplace(itr, node);
}

void MallocBlockVector::RemoveBack()
{
	this->Remove(this->end() - 1);
}

void MallocBlockVector::Remove(Iterator idx)
{
	--m_Count;
	for (; idx != this->end(); ++idx)
	{
		*idx = *(idx + 1);
	}
}

void MallocBlockVector::Sort()
{
	auto partition =
		[](MallocNode array[], int64_t low, int64_t high)->int64_t
		{
		uint64_t pivot = (uintptr_t)array[high].block;
		int64_t i = low - 1;

		for (int64_t j = low; j < high; j++)
		{
			if ((uintptr_t)array[j].block <= pivot)
			{
				i++;
				swap(&array[i], &array[j]);
			}
		}
		swap(&array[i + 1], &array[high]);
		return i + 1;
	};

	auto quicksort =
		[](
				MallocNode array[],
				int64_t low,
				int64_t high,
				auto partition,
				auto quicksort)->MallocNode*
		{
			if (low >= high) return array;
			int64_t part = partition(array, low, high);
			quicksort(array, low, part - 1, partition, quicksort);
			quicksort(array, part + 1, high, partition, quicksort);
			return array;
		};

	quicksort(m_Block, 0, m_Count - 1, partition, quicksort);
}

MallocBlockVector::Iterator MallocBlockVector::Find(void* addr)
{
	auto search =
		[](
			MallocNode array[],
			void* addr,
			int64_t low,
			int64_t high,
			auto& search)->int64_t
		{
			if (low > high)
				return -1;

			int64_t mid = (low + high) / 2;
			if (array[mid].block == addr)
				return mid;
			else if (array[mid].block < addr)
				return search(array, addr, mid + 1, high, search);
			else
				return search(array, addr, low, mid - 1, search);
		};

	int idx = 0;
	if ((idx = search(this->begin(), addr, 0, m_Count - 1, search)) == -1)
		return this->end();

	return this->begin() + idx;
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
		if (m_Size < 64)
		{
			void *newBlock = realloc(m_Block, 64);

			if (!newBlock)
				return;

			m_Size = 64;
			m_Block = reinterpret_cast<MallocNode*>(newBlock);
		}
		else
		{
			size_t newSize = m_Size + (m_Size / 2);
			void *newBlock = realloc(m_Block, newSize);

			if (!newBlock)
				return;

			m_Size = newSize;
			m_Block = reinterpret_cast<MallocNode*>(newBlock);
		}
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
	sortMemoryMap(&s_MemoryMap);

	s_MallocAllocations = MallocBlockVector(
			reinterpret_cast<void*>(0x2000000),
			sizeof(MallocNode) * 1024);
}

void *malloc(size_t size)
{
	// size == 0 is invalid, return nullptr
	if (size == 0)
		return nullptr; //TODO: think about throwing an exception

	
	for (MallocBlockVector::Iterator itr = s_MallocAllocations.begin();
			itr != s_MallocAllocations.end();
			++itr)
	{
		if ((uintptr_t)itr->block + itr->blockLength + size <=
				(uintptr_t)((itr + 1)->block))
		{
			void* addr = (void*)((uintptr_t)itr->block + itr->blockLength);
			s_MallocAllocations.Insert({ size, addr });
			return addr;
		}
	}
	

	MallocBlockVector::Iterator itr = s_MallocAllocations.end() - 1;
	uintptr_t endOfMallocBlock = (uintptr_t)itr->block + itr->blockLength;
	

	if (!memoryInUsableRange(&s_MemoryMap, endOfMallocBlock, size))
		return nullptr; //TODO: think about throwing an exception

	s_MallocAllocations.Insert({ size, (void*)endOfMallocBlock });
	return (void*)endOfMallocBlock;
}

void *realloc(void *addr, size_t size)
{
	// If addr == nullptr, return result of malloc, otherwise continue realloc
	// routine.
	if (addr != nullptr)
	{
		MallocNode *node;
		
		// Find the node from allocating addr
		if ((node = s_MallocAllocations.Find(addr)) ==
				s_MallocAllocations.end())
			// If the node doest exist return nullptr
			return nullptr; //TODO: think about throwing an exception

		// If new size is smaller than or equal to the original size reassign
		// the size of the block to the new size and return addr.
		if (size <= node->blockLength)
		{
			node->blockLength = size;
			return addr;
		}

		// If there is space between this allocation and the next allocation,
		// or if this allocation is the last one, expand the block to the new
		// size. Otherwise allocate a new block of the desired size, copy the
		// contents from the old block to the new one, then free the old block.
		if ((node + 1) == s_MallocAllocations.end() ||
				(uintptr_t)node->block + size <= (uintptr_t)((node + 1)->block))
		{
			node->blockLength = size;
			return addr;
		}
		else
		{
			void *newBlock = malloc(size);
			memcpy(newBlock, addr, node->blockLength);
			free(addr);
			return newBlock;
		}
	}
	return malloc(size);
}

void free(void *addr)
{
	MallocBlockVector::Iterator itr;
	if ((itr = s_MallocAllocations.Find(addr)) != s_MallocAllocations.end())
		s_MallocAllocations.Remove(itr);
}

//MallocLinkedList::ReverseIterator MallocLinkedList::rbegin()
//{
//}
//
//MallocLinkedList::ReverseIterator MallocLinkedList::rend()
//{
//}

