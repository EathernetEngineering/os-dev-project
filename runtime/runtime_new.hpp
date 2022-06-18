#ifndef _RUNTIME_NEW_HPP
#define _RUNTIME_NEW_HPP

#if defined(__cpp_aligned_new)
namespace std
{
	enum calss align_val_t : size_t {};
}
#endif // defined(__cpp_aligned_new)

#endif // _RUNTIME_NEW_HPP

