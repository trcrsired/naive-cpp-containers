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
#if __has_cpp_attribute(__gnu__::__malloc__)
[[__gnu__::__malloc__]]
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
#if __has_cpp_attribute(__gnu__::__const__)
[[__gnu__::__const__]]
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
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(__gnu__::__dllimport__)
[[__gnu__::__dllimport__]]
#endif
extern void* __stdcall HeapReAlloc(void*,::std::uint_least32_t,void*,::std::size_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT_LEAST32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
__asm__("HeapReAlloc@16")
#else
__asm__("_HeapReAlloc@16")
#endif
#else
__asm__("HeapReAlloc")
#endif
#endif
;

}

namespace details
{
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
#if __has_cpp_attribute(__gnu__::__malloc__)
[[__gnu__::__malloc__]]
#endif
inline void* win32_heapalloc_impl(::std::size_t to_allocate) noexcept
{
	if(to_allocate==0)
	{
		to_allocate=1;
	}
	auto p{::ncc::win32::HeapAlloc(
		::ncc::win32::GetProcessHeap(),
		0u,to_allocate)};
	if(p==nullptr)
	{
		::ncc::details::fast_terminate();
	}
	return p;
}

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
inline void* win32_heapalloc_zero_impl(::std::size_t to_allocate) noexcept
{
	if(to_allocate==0)
	{
		to_allocate=1;
	}
	auto p{::ncc::win32::HeapAlloc(
		::ncc::win32::GetProcessHeap(),
		0x00000008u,to_allocate)};
	if(p==nullptr)
	{
		::ncc::details::fast_terminate();
	}
	return p;
}

inline void win32_heapfree_impl(void* addr) noexcept
{
	if(addr==nullptr)
		return;
	::ncc::win32::HeapFree(
		::ncc::win32::GetProcessHeap(),
		0u,addr);
}

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
inline void* win32_heaprealloc_impl(void* addr,::std::size_t n) noexcept
{
	if(n==0)
	{
		n=1;
	}
	if(addr==nullptr)
#if __has_cpp_attribute(unlikely)
	[[unlikely]]
#endif
	{
		return win32_heapalloc_impl(n);
	}
	auto p{::ncc::win32::HeapReAlloc(
		::ncc::win32::GetProcessHeap(),
		0u,addr,n)};
	if(p==nullptr)
	{
		::ncc::details::fast_terminate();
	}
	return p;
}

}

class win32_heapalloc_allocator
{
public:
	template<typename T>
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
	static inline constexpr T* allocate(::std::size_t n) noexcept
	{
		if constexpr(sizeof(T)!=1)
		{
			constexpr std::size_t mx{::std::numeric_limits<::std::size_t>::max()/sizeof(T)};
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
			auto p{::ncc::details::win32_heapalloc_impl(to_allocate)};
			return reinterpret_cast<T*>(p);
		}
	}
	template<typename T>
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
	static inline constexpr T* allocate_zero(::std::size_t n) noexcept
	{
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
			::std::size_t const to_allocate{n*sizeof(T)};
			auto ptr{::operator new(to_allocate)};
			auto cstr{static_cast<char unsigned*>(ptr)};
			for(auto i{cstr},e{cstr+to_allocate};i!=e;++i)
			{
				*i=0;
			}
			return static_cast<T*>(ptr);
		}
		else
		{
		{
			auto p{::ncc::details::win32_heapalloc_zero_impl(to_allocate)};
			return reinterpret_cast<T*>(p);
		}
		}
	}
	template<typename T>
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
	static inline constexpr T* reallocate(T* ptr,::std::size_t n) noexcept
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
			auto p{::ncc::details::win32_heaprealloc_impl(ptr,to_allocate)};
			return reinterpret_cast<T*>(p);
		}
	}
	template<typename T>
	static inline constexpr void deallocate(T* p,::std::size_t) noexcept
	{
		if consteval
		{
			::operator delete(p);
		}
		else
		{
			::ncc::details::win32_heapfree_impl(p);
		}
	}
};

}
