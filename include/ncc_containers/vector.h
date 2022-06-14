#pragma once
#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning( disable : 4514 )
#pragma warning( disable : 4668 )
#pragma warning( disable : 4710 )
#pragma warning( disable : 5045 )
#endif
#include"impl/common.h"

namespace ncc
{

namespace freestanding
{

template<::std::movable T,::ncc::ncc_allocator alloc>
class vector
{
public:
	using allocator_type = alloc;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = value_type const*;

	using reference = value_type&;
	using const_reference = value_type const&;

	using iterator = value_type*;
	using const_iterator = value_type const*;

	using size_type = ::std::size_t;
	using difference_type = ::std::ptrdiff_t;

	pointer begin_ptr{};
	pointer curr_ptr{};
	pointer end_ptr{};
#ifndef __INTELLISENSE__
#if __has_cpp_attribute(no_unique_address)
	[[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
	[[msvc::no_unique_address]]
#endif
#endif
	allocator_type allocator;

	constexpr iterator begin() noexcept
	{
		return begin_ptr;
	}
	constexpr iterator end() noexcept
	{
		return curr_ptr;
	}
	constexpr const_iterator begin() const noexcept
	{
		return begin_ptr;
	}
	constexpr const_iterator end() const noexcept
	{
		return curr_ptr;
	}
	constexpr const_iterator cbegin() const noexcept
	{
		return begin_ptr;
	}
	constexpr const_iterator cend() const noexcept
	{
		return curr_ptr;
	}

	constexpr pointer data() noexcept
	{
		return begin_ptr;
	}
	constexpr const_pointer data() const noexcept
	{
		return begin_ptr;
	}
	constexpr bool is_empty() const noexcept
	{
		return begin_ptr == curr_ptr;
	}

	constexpr bool empty() const noexcept
	{
		return begin_ptr == curr_ptr;
	}
	constexpr void clear() noexcept
	{
		if constexpr(!::std::is_trivially_copyable_v<value_type>)
		{
			for(auto old_i{begin_ptr},old_e{curr_ptr};old_i!=old_e;++old_i)
			{
				old_i->~value_type();
			}
		}
		curr_ptr = begin_ptr;
	}
	constexpr size_type size() const noexcept
	{
		return static_cast<size_type>(curr_ptr-begin_ptr);
	}
	constexpr size_type capacity() const noexcept
	{
		return static_cast<size_type>(end_ptr-begin_ptr);
	}

	explicit constexpr vector() noexcept = default;

private:
	void destroy()
	{
		clear();
		allocator.template deallocate<value_type>(begin_ptr,static_cast<std::size_t>(end_ptr-begin_ptr));
	}
	struct run_destroy
	{
		vector* thisvec{};
		constexpr run_destroy() noexcept=default;
		explicit constexpr run_destroy(vector* p) noexcept:thisvec(p){}
		run_destroy(run_destroy const&)=delete;
		run_destroy& operator=(run_destroy const&)=delete;
		constexpr ~run_destroy()
		{
			if(thisvec)
			{
				thisvec->destroy();
			}
		}
	};
public:

	explicit constexpr vector(size_type n) noexcept(::std::is_scalar_v<value_type>)
	{
		if constexpr(::std::is_scalar_v<value_type>)
		{
			begin_ptr=allocator.template allocate_zero<value_type>(n);
			end_ptr=curr_ptr=begin_ptr+n;
		}
		else
		{
			auto e{this->end_ptr=(this->curr_ptr=this->begin_ptr=
				allocator.template allocate<value_type>(n))+n};
			run_destroy des(this);
			for(;this->curr_ptr!=e;++this->curr_ptr)
			{
				new (this->curr_ptr) value_type;
			}
			des.thisvec=nullptr;
		}
	}


	constexpr vector(vector const& vec) requires(::std::copyable<value_type>)
		: allocator(vec.allocator)
	{
		if constexpr(::std::is_trivially_copyable_v<value_type>)
		{
			std::size_t const vecsize{static_cast<std::size_t>(vec.curr_ptr-vec.begin_ptr)};
			std::size_t n{vecsize*sizeof(value_type)};
			if(n==0)
			{
				return;
			}
			begin_ptr=allocator.template allocate<value_type>(vecsize);
#if defined(__has_builtin)
#if __has_builtin(__builtin_memcpy)
			__builtin_memcpy(begin_ptr,vec.begin_ptr,n);
#else
			std::memcpy(begin_ptr,vec.begin_ptr,n);
#endif
#else
			std::memcpy(begin_ptr,vec.begin_ptr,n);
#endif
			end_ptr=curr_ptr=begin_ptr+vecsize;
		}
		else
		{
			std::size_t const vecsize{static_cast<std::size_t>(vec.curr_ptr-vec.begin_ptr)};
			std::size_t n{vecsize*sizeof(value_type)};
			if(n==0)
			{
				return;
			}
			this->begin_ptr=allocator.template allocate<value_type>(vecsize);
			run_destroy des(this);
			auto e{begin_ptr+vecsize};
			this->end_ptr=e;
			for(;this->curr_ptr!=e;++this->curr_ptr)
			{
				new (this->curr_ptr) value_type;
			}
			des.thisvec=nullptr;
		}
	}
	constexpr vector(vector const& vec) = delete;
	constexpr vector& operator=(vector const& vec) requires(::std::copyable<value_type>)
	{
		vector newvec(vec);
		this->operator=(::ncc::details::move(newvec));
		return *this;
	}
	constexpr vector& operator=(vector const& vec) = delete;
	constexpr vector(vector&& vec) noexcept:begin_ptr(vec.begin_ptr),curr_ptr(vec.curr_ptr),
		end_ptr(vec.end_ptr),allocator(::ncc::details::move(vec.allocator))
	{
		vec.end_ptr=vec.curr_ptr=vec.begin_ptr=nullptr;
	}
	constexpr vector& operator=(vector&& vec) noexcept
	{
		this->destroy();
		this->begin_ptr=vec.begin_ptr;
		this->curr_ptr=vec.curr_ptr;
		this->end_ptr=vec.end_ptr;
		this->allocator=::ncc::details::move(vec.allocator);
		vec.end_ptr=vec.curr_ptr=vec.begin_ptr=nullptr;
		return *this;
	}
	constexpr ~vector()
	{
		destroy();
	}

	template<typename... Args>
	requires std::constructible_from<value_type,Args...>
	constexpr reference emplace_back_unchecked(Args&& ...args)
	{
		auto p{::new (curr_ptr) value_type(::ncc::details::forward<Args>(args)...)};
		++curr_ptr;
		return *p;
	}

private:
	inline constexpr void grow_to_size_impl(size_type newcap) noexcept
	{
		std::size_t const cap{static_cast<size_type>(end_ptr-begin_ptr)};
		std::size_t const old_size{static_cast<std::size_t>(curr_ptr-begin_ptr)};
		if constexpr(::std::is_trivially_copyable_v<value_type>)
		{
			begin_ptr=allocator.template reallocate<value_type>(begin_ptr,newcap);
			curr_ptr=begin_ptr+old_size;
			end_ptr=begin_ptr+newcap;
		}
		else
		{
			auto new_begin_ptr=allocator.template allocate<value_type>(begin_ptr,newcap);
			auto new_i{new_begin_ptr};
			for(auto old_i{begin_ptr},old_e{curr_ptr};old_i!=old_e;++old_i)
			{
				new (new_i) value_type(::ncc::details::move(*old_i));
				old_i->~value_type();
				++new_i;
			}
			allocator.template deallocate<value_type>(begin_ptr,cap);
			begin_ptr=new_begin_ptr;
			curr_ptr=new_i;
			end_ptr=new_begin_ptr+newcap;
		}
	}
	inline constexpr void grow_twice_impl() noexcept
	{
		std::size_t const cap{static_cast<size_type>(end_ptr-begin_ptr)};
		constexpr std::size_t mx_value{::std::numeric_limits<std::size_t>::max()/sizeof(value_type)};
		constexpr std::size_t mx_half_value{mx_value/2};
		if(cap==mx_value)
		{
			::ncc::details::fast_terminate();
		}
		std::size_t newcap;
		if(cap>mx_half_value)
		{
			newcap=mx_value;
		}
		else if(cap==0)
		{
			newcap = 1;
		}
		else
		{
			constexpr std::size_t two{2};
			newcap=static_cast<std::size_t>(cap*two);
		}
		grow_to_size_impl(newcap);
	}
public:
	constexpr void reserve(size_type n) noexcept
	{
		if(n<=static_cast<std::size_t>(end_ptr-begin_ptr))
		{
			return;
		}
		grow_to_size_impl(n);
	}

	constexpr void shrink_to_fit() noexcept
	{
		if(curr_ptr==end_ptr)
		{
			return;
		}
		grow_to_size_impl(static_cast<std::size_t>(curr_ptr-begin_ptr));
	}
	constexpr void pop_back() noexcept
	{
#if defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && (defined(_GLIBCXX_ASSERTIONS) || defined(_GLIBCXX_DEBUG))
		if(begin_ptr==curr_ptr)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		{
			__builtin_trap();
		}
#endif
		(--curr_ptr)->~value_type();
	}
	constexpr reference operator[](size_type pos) noexcept
	{
#if defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && (defined(_GLIBCXX_ASSERTIONS) || defined(_GLIBCXX_DEBUG))
		if(static_cast<std::size_t>(end_ptr-begin_ptr)<=pos)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		{
			__builtin_trap();
		}
#endif
		return begin_ptr[pos];
	}
	constexpr const_reference operator[](size_type pos) const noexcept
	{
#if defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && (defined(_GLIBCXX_ASSERTIONS) || defined(_GLIBCXX_DEBUG))
		if(static_cast<std::size_t>(end_ptr-begin_ptr)<=pos)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		{
			__builtin_trap();
		}
#endif
		return begin_ptr[pos];
	}

	constexpr const_reference front() const noexcept
	{
#if defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && (defined(_GLIBCXX_ASSERTIONS) || defined(_GLIBCXX_DEBUG))
		if(begin_ptr==curr_ptr)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		{
			__builtin_trap();
		}
#endif
		return *begin_ptr;
	}

	constexpr reference front() noexcept
	{
#if defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && (defined(_GLIBCXX_ASSERTIONS) || defined(_GLIBCXX_DEBUG))
		if(begin_ptr==curr_ptr)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		{
			__builtin_trap();
		}
#endif
		return *begin_ptr;
	}

	constexpr const_reference back() const noexcept
	{
#if defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && (defined(_GLIBCXX_ASSERTIONS) || defined(_GLIBCXX_DEBUG))
		if(begin_ptr==curr_ptr)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		{
			__builtin_trap();
		}
#endif
		return curr_ptr[-1];
	}
	constexpr reference back() noexcept
	{
#if defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && (defined(_GLIBCXX_ASSERTIONS) || defined(_GLIBCXX_DEBUG))
		if(begin_ptr==curr_ptr)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		{
			__builtin_trap();
		}
#endif
		return curr_ptr[-1];
	}
	template<typename... Args>
	requires std::constructible_from<value_type,Args...>
#if __has_cpp_attribute(__gnu__::__always_inline__)
[[__gnu__::__always_inline__]]
#endif
	constexpr reference emplace_back(Args&& ...args)
	{
		if(curr_ptr==end_ptr)
#if __has_cpp_attribute(unlikely)
		[[unlikely]]
#endif
		{
			grow_twice_impl();
		}
		auto p{new (curr_ptr) value_type(::ncc::details::forward<Args>(args)...)};
		++curr_ptr;
		return *p;
	}
};

}

#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
template<::std::movable T,::ncc::ncc_allocator alloc=::ncc::native_allocator>
using vector = ::ncc::freestanding::vector<T,alloc>;
#endif

}
#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif
