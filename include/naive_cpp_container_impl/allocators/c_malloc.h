#pragma once

namespace ncc
{

template<typename T>
class c_malloc_allocator
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
#if 0
	static inline constexpr T* reallocate(T* p,::std::size_t n) noexcept
	{

	}
	static inline constexpr T* allocate_zero(::std::size_t n) noexcept
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
			auto p{calloc(to_allocate)};
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

#endif

	static inline constexpr void deallocate(T* p,::std::size_t) noexcept
	{
		if consteval
		{
			::operator delete(p);
		}
		else
		{
			if constexpr(alignof(T)>alignof(::std::max_align_t))
			{
				::aligned_free(p,alignof(T));
			}
			else
			{
				free(p);
			}
		}
	}
};

}
