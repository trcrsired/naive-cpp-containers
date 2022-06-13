#pragma once

#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)

#if (defined(_WIN32) || defined(__CYGWIN__)) && !defined(__WINE__)
#include"win32_heapalloc.h"
#endif
#include"c_malloc.h"
#endif
#include"custom.h"

namespace ncc
{

template<typename T>
using native_allocator =
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__WINE__)
win32_heapalloc_allocator<T>
#else
c_malloc_allocator<T>
#endif
#else
custom_allocator<T>
#endif
;

}
