#pragma once

namespace ncc
{

template<typename T>
class nt_rtlallocateheap_allocator
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
			auto p{::ncc::details::nt_rtlallocateheap_impl(to_allocate)};
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
			::ncc::details::nt_rtlallocateheap(p);
		}
	}
};

}
