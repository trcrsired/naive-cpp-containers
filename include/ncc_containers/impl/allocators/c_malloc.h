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

template<typename T>
class c_malloc_allocator
{
	using value_type = T;
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
		::std::size_t const to_allocate{n*sizeof(T)};
		if consteval
		{
			return ::operator new(to_allocate);
		}
		else
		{
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
		::std::size_t const to_allocate{n*sizeof(T)};
		if consteval
		{
			::operator delete(p);
			return ::operator new(to_allocate);
		}
		else
		{
			p=realloc(p,to_allocate);
			if(p==nullptr)
			{
				::ncc::details::fast_terminate();
			}
			return p;
		}
	}
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
			return ::operator new(to_allocate);
		}
		else
		{
		if constexpr(alignof(T)>alignof(::std::max_align_t))
		{
			::std::size_t const to_allocate{n*sizeof(T)};
			auto p{::ncc::details::noexcept_call(::aligned_alloc,alignof(T),to_allocate)};
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
