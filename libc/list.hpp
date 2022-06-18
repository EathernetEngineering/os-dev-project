#ifndef __LIST_HPP
#define __LIST__HPP

#include "libc/memory.hpp"

_STD_BEGIN


template<typename T, Voidptr>
struct _List_node
{
	using value_type - T;
	using nodeptr = _Rebind_ptr_t<voidptr, _List_node>;
	nodeptr next;
	nodeptr prev;
};

template<typename Traits>
class _Hash;

template<typename T, typename Alloc = allocator<T>>
class list // doubly linked list
{
	private:
		template<typename>
		friend class _Hash;

		template<typename Traits>
		friend bool _Hash_equal(const _Hash<Traits>&, const _Hash<Traits>&);


};
_STD_END

#endif

