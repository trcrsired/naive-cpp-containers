#pragma once
#if defined(__has_builtin)
#if !__has_builtin(__builtin_memset)
#include<cstring>
#endif
#else
#include<cstring>
#endif

namespace ncc
{

class c_malloc_allocator
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
			constexpr std::size_t mx{::std::numeric_limits<std::size_t>::max()/sizeof(T)};
			if(mx<n)
			{
				::ncc::details::fast_terminate();
			}
		}
		if(n==0)
		{
			n=1;
		}
		::std::size_t const to_allocate{n*sizeof(T)};
		if consteval
		{
			return ::operator new(to_allocate);
		}
		else
		{
		if constexpr(alignof(T)>alignof(::std::max_align_t))
		{
			auto p{::ncc::details::noexcept_call(
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__WINE__)
				::_aligned_malloc
#else
				::aligned_alloc
#endif
				,alignof(T),to_allocate)};
			if(p==nullptr)
			{
				::ncc::details::fast_terminate();
			}
			return reinterpret_cast<T*>(p);
		}
		else
		{
			auto p{malloc(to_allocate)};
			if(p==nullptr)
			{
				::ncc::details::fast_terminate();
			}
			if constexpr(alignof(T)>alignof(::std::max_align_t))
			{
				if(reinterpret_cast<std::size_t>(p)%alignof(T))
				{
					::ncc::details::fast_terminate();
				}
			}
			return p;
		}
		}
	}
	template<typename T>
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
	static inline constexpr T* reallocate(T* p,::std::size_t n) noexcept
	{
		if constexpr(sizeof(T)!=1)
		{
			constexpr std::size_t mx{::std::numeric_limits<std::size_t>::max()/sizeof(T)};
			if(mx<n)
			{
				::ncc::details::fast_terminate();
			}
		}
		if(n==0)
		{
			n=1;
		}
		::std::size_t const to_allocate{n*sizeof(T)};
		if consteval
		{
			::operator delete(p);
			return static_cast<T*>(::operator new(to_allocate));
		}
		else
		{
			p=static_cast<T*>(realloc(p,to_allocate));
			if(p==nullptr)
			{
				::ncc::details::fast_terminate();
			}
			return p;
		}
	}
	template<typename T>
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
#endif
	static inline constexpr T* allocate_zero(::std::size_t n) noexcept
	{
		if constexpr(sizeof(T)!=1)
		{
			constexpr std::size_t mx{::std::numeric_limits<std::size_t>::max()/sizeof(T)};
			if(mx<n)
			{
				::ncc::details::fast_terminate();
			}
		}
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
			if(n==0)
			{
				n=1;
			}
		if constexpr(alignof(T)>alignof(::std::max_align_t))
		{
			::std::size_t const to_allocate{n*sizeof(T)};
			auto p{::ncc::details::noexcept_call(
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__WINE__)
				::_aligned_malloc
#else
				::aligned_alloc
#endif
				,alignof(T),to_allocate)};
			if(p==nullptr)
			{
				::ncc::details::fast_terminate();
			}
#if defined(__has_builtin)
#if __has_builtin(__builtin_memset)
			__builtin_memset(p,0,to_allocate);
#else
			std::memset(p,0,to_allocate);
#endif
#else
			std::memset(p,0,to_allocate);
#endif
			return reinterpret_cast<T*>(p);
		}
		else
		{
			auto p{calloc(n,sizeof(T))};
			if(p==nullptr)
			{
				::ncc::details::fast_terminate();
			}
			return p;
		}
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
			free(p);
		}
	}
};

}
