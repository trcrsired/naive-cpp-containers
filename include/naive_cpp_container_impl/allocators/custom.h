#pragma once

namespace ncc
{

void* ncc_custom_allocate(std::size_t) noexcept;//never return nullptr
void ncc_custom_deallocate(void*,std::size_t) noexcept;
void* ncc_custom_aligned_allocate(std::size_t,std::size_t) noexcept;//never return nullptr
void ncc_custom_aligned_deallocate(void*,std::size_t,std::size_t) noexcept;

template<typename T>
class ncc_custom_allocator
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
			if constexpr(alignof(T)>alignof(::std::max_align_t))
			{
				return ncc_custom_aligned_allocate(alignof(T),to_allocate);
			}
			else
			{
				return ncc_custom_allocate(to_allocate);
			}
		}
	}
	static inline constexpr void deallocate(T* ptr,::std::size_t n) noexcept
	{
		if consteval
		{
			::operator delete(p);
		}
		else
		{
			std::size_t const to_allocate{n*sizeof(T)};
			if constexpr(alignof(T)>alignof(::std::max_align_t))
			{
				return ncc_custom_aligned_deallocate(ptr,alignof(T),to_allocate);
			}
			else
			{
				return ncc_custom_deallocate(ptr,to_allocate);
			}
		}
	}
}

}
