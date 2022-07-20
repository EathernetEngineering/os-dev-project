#ifndef _KLIBC_ALGORITHM_HPP
#define _KLIBC_ALGORITHM_HPP

template<typename Ittr, typename Func>
Func foreach(Ittr first, Ittr last, Func f)
{
	for (; first != last; ++first)
	{
		f(*first);
	}
	return f;
}

#endif

