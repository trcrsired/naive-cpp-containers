#pragma once

namespace ncc
{

extern void* ncc_custom_allocate(std::size_t) noexcept;//never return nullptr
extern void ncc_custom_deallocate(void*,std::size_t) noexcept;
extern void* ncc_custom_aligned_allocate(std::size_t,std::size_t) noexcept;//never return nullptr
extern void ncc_custom_aligned_deallocate(void*,std::size_t,std::size_t) noexcept;

extern void* ncc_custom_allocate_zero(std::size_t) noexcept;//never return nullptr
extern void* ncc_custom_aligned_allocate_zero(std::size_t,std::size_t) noexcept;//never return nullptr

extern void* ncc_custom_reallocate(void*,std::size_t) noexcept;//never return nullptr
extern void* ncc_custom_aligned_reallocate(void*,std::size_t,std::size_t) noexcept;//never return nullptr


class ncc_custom_allocator
{
public:
	template<typename T>
	static inline constexpr T* allocate(::std::size_t n) noexcept
	{
		if constexpr(sizeof(T)!=1)
		{
			constexpr std::size_t mx{::std::numeric_limits<T>::max()/sizeof(T)};
			if(mx<n)
			{
				::ncc::details::fast_terminate();
			}
		}
		::std::size_t const to_allocate{n*sizeof(T)};
#if (__cpp_if_consteval >= 202106L || __cpp_lib_is_constant_evaluated >= 201811L) && __cpp_constexpr_dynamic_alloc >= 201907L
#if __cpp_if_consteval >= 202106L
		if consteval
#else
		if(__builtin_is_constant_evaluated())
#endif
		{
			return static_cast<T*>(::operator new(to_allocate));
		}
		else
#endif
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
	template<typename T>
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
		::std::size_t const to_allocate{n*sizeof(T)};
#if (__cpp_if_consteval >= 202106L || __cpp_lib_is_constant_evaluated >= 201811L) && __cpp_constexpr_dynamic_alloc >= 201907L
#if __cpp_if_consteval >= 202106L
		if consteval
#else
		if(__builtin_is_constant_evaluated())
#endif
		{
			auto ptr{::operator new(to_allocate)};
			auto cstr{static_cast<char unsigned*>(ptr)};
			for(auto i{cstr},e{cstr+to_allocate};i!=e;++i)
			{
				*i=0;
			}
			return static_cast<T*>(ptr);
		}
		else
#endif
		{
			if constexpr(alignof(T)>alignof(::std::max_align_t))
			{
				return reinterpret_cast<T*>(ncc_custom_aligned_allocate_zero(alignof(T),to_allocate));
			}
			else
			{
				return reinterpret_cast<T*>(ncc_custom_allocate_zero(to_allocate));
			}
		}
	}
	template<typename T>
	static inline constexpr void* reallocate(T* ptr,::std::size_t n) noexcept
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
#if (__cpp_if_consteval >= 202106L || __cpp_lib_is_constant_evaluated >= 201811L) && __cpp_constexpr_dynamic_alloc >= 201907L
#if __cpp_if_consteval >= 202106L
		if consteval
#else
		if(__builtin_is_constant_evaluated())
#endif
		{
			::operator delete(ptr);
			return static_cast<T*>(::operator new(to_allocate));
		}
		else
#endif
		{
			if constexpr(alignof(T)>alignof(::std::max_align_t))
			{
				return reinterpret_cast<T*>(ncc_custom_aligned_reallocate(ptr,alignof(T),to_allocate));
			}
			else
			{
				return reinterpret_cast<T*>(ncc_custom_reallocate(ptr,to_allocate));
			}
		}
	}
	template<typename T>
	static inline constexpr void deallocate(T* ptr,::std::size_t n) noexcept
	{
#if (__cpp_if_consteval >= 202106L || __cpp_lib_is_constant_evaluated >= 201811L) && __cpp_constexpr_dynamic_alloc >= 201907L
#if __cpp_if_consteval >= 202106L
		if consteval
#else
		if(__builtin_is_constant_evaluated())
#endif
		{
			::operator delete(ptr);
		}
		else
#endif
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
};

}
