#ifndef __YVALS_CORE
#define __YVALS_CORE

#ifndef _HAS_CXX17
    #if __cplusplus > 201402L
        #define _HAS_CXX17 1
    #else // ^^^ __cplusplus > 201402L / vvv !__cplusplus > 201402L
        #define _HAS_CXX17 0
    #endif // __cplusplus > 201402L
#endif

#ifndef _HAS_CXX20
    #if _HAS_CXX17 && __cplusplus > 201703L
        #define _HAS_CXX20 1
    #else // ^^^ __cplusplus > 201703L / vvv !__cplusplus > 201703L
        #define _HAS_CXX20 0
    #endif // __cplusplus > 201703L
#endif

#ifndef _HAS_CXX23
    #if _HAS_CXX20 && __cplusplus > 202002L
        #define _HAS_CXX23 1
    #else // ^^^ __cplusplus > 202002L / vvv !__cplusplus > 202002L
        #define _HAS_CXX23 0
    #endif // __cplusplus > 202002L
#endif

#if _HAS_CXX20 && !_HAS_CXX17
    #error _HAS_CXX20 must imply _HAS_CXX17
#endif // _HAS_CXX20 && !_HAS_CXX17

#if _HAS_CXX23 && !_HAS_CXX20
    #error _HAS_CXX23 must imply _HAS_CXX20
#endif // _HAS_CXX23 && !_HAS_CXX20

#if _HAS_NODISCARD
	#define _NODISCARD __attribute__((nodiscard))
#else // ^^^ _HAS_NODISCARD / vvv !_HAS_NODISCARD
	#define _NODISCARD
#endif // _HAS_NODISCARD

#if _HAS_CXX20
#define _CONSTEXPR20 constexpr
#else // ^^^ _HAS_CXX20 / vvv !_HAS_CXX20
#define _CONSTEXPR20 inline
#endif // _HAS_CXX20

#define _STD_BEGIN namespace std {
#define _STD_END   }
#define _STD ::std::
#define _CHRONO ::std::chrono::
#define _RANGES ::std::ranges::

#define _CSTD ::

#if _HAS_CXX17 && !defined(_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING) \
    && !defined(_SILENCE_ALL_CXX17_DEPECATION_WARNINGS)
#define _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS                                                           \
    __attribute__((deprecated("warning STL4010: "                                                        \
                              "Various members of std::allocator are deprecated in C++17. "              \
                              "Use std::allocator_traits instead of accessing these members directly. "  \
                              "You can define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING " \
                              "or _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS to acknowledge that you reciived this waning.")))
#endif //_HAS_CXX17 && !defined(_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)
       // && !defined(_SILENCE_ALL_CXX17_DEPECATION_WARNINGS)

#if _HAS_CXX17
#define _INLINE_VAR inline
#else // ^^^ _HAS_CXX20 / vvv !_HAS_CXX20
#define _INLINE_VAR
#endif // _HAS_CXX17

#ifndef _HAS_FEATURES_REMOVED_IN_CXX20
#define _HAS_FEATURES_REMOVED_IN_CXX20 (!_HAS_CXX20)
#endif // _HAS_FEATURES_REMOVED_IN_CXX20

#ifndef _HAS_DEPRECATED_ALLOCATOR_MEMBERS
#define _HAS_DEPRECATED_ALLOCATOR_MEMBERS (_HAS_FEATURES_REMOVED_IN_CXX20)
#endif // _HAS_DEPRECATED_ALLOCATOR_MEMBERS

#endif // _YVALS_CORE_HPP

