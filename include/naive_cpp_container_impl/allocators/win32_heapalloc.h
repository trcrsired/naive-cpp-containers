#pragma once

namespace ncc
{

namespace win32
{
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(__gnu__::__dllimport__)
[[__gnu__::__dllimport__]]
#endif
extern void* __stdcall HeapAlloc(void*,::std::uint_least32_t,::std::size_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT_LEAST32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
__asm__("HeapAlloc@12")
#else
__asm__("_HeapAlloc@12")
#endif
#else
__asm__("HeapAlloc")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(__gnu__::__dllimport__)
[[__gnu__::__dllimport__]]
#endif
extern int __stdcall HeapFree(void*,::std::uint_least32_t,void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT_LEAST32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
__asm__("HeapFree@12")
#else
__asm__("_HeapFree@12")
#endif
#else
__asm__("HeapFree")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(__gnu__::__dllimport__)
[[__gnu__::__dllimport__]]
#endif
extern void* __stdcall GetProcessHeap() noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT_LEAST32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
__asm__("GetProcessHeap@0")
#else
__asm__("_GetProcessHeap@0")
#endif
#else
__asm__("GetProcessHeap")
#endif
#endif
;

}

namespace details
{
inline void* win32_heapalloc_impl(::std::size_t to_allocate) noexcept
{
	auto p{::ncc::win32::HeapAlloc(
		::ncc::win32::GetProcessHeap(),
		0u,to_allocate)};
	if(p==nullptr)
	{
		::ncc::details::fast_terminate();
	}
	return p;
}
inline void win32_heapfree_impl(void* addr) noexcept
{
	::ncc::win32::HeapFree(
		::ncc::win32::GetProcessHeap(),
		0u,addr);
}

}

template<typename T>
class win32_heapalloc_allocator
{
	static inline constexpr T* allocate(::std::size_t n) noexcept
	{
		if constexpr(sizeof(T)!=1)
		{
			constexpr std::size_t mx{SIZE_MAX/sizeof(T)};
			if(mx<n)
			{
				::ncc::details::fast_terminate();
			}
		}
		::std::size_t const to_allocate{n*sizeof(T)};
		if consteval
		{
			return static_cast<T*>(::operator new(to_allocate));
		}
		else
		{

#if 0
		if constexpr(alignof(T)>alignof(::std::max_align_t))
		{
			auto p{::ncc::details::noexcept_call(::aligned_alloc,alignof(T),to_allocate)};
			if(p==nullptr)
			{
				::ncc::details::fast_terminate();
			}
			return reinterpret_cast<T*>(p);
		}
		else
#endif
		{
			auto p{::ncc::details::win32_heapalloc_impl(to_allocate)};
#if 0
			if constexpr(alignof(T)>alignof(::std::max_align_t))
			{
				if(reinterpret_cast<std::size_t>(p)%alignof(T))
				{
					::ncc::details::fast_terminate();
				}
			}
#endif
			return reinterpret_cast<T*>(p);
		}
		}
	}
	static inline constexpr void deallocate(T* p,::std::size_t) noexcept
	{
		if consteval
		{
			::operator delete(p);
		}
		else
		{
#if 0
		if constexpr(alignof(T)>alignof(::std::max_align_t))
		{
		//	::aligned_free(p,alignof(T));
		}
		else
#endif
		{
			::ncc::details::win32_heapfree_impl(p);
		}
		}
	}
};

}
