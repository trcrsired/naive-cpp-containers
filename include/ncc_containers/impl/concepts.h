#pragma once

namespace ncc
{

template<typename T>
concept ncc_allocator=sizeof(T)<=(sizeof(void*)*2)&&::std::is_trivially_copyable_v<T>&&requires(T t,typename T::value_type* ptr,std::size_t n)
{
	{t.allocate(n)}->::std::same_as<typename T::value_type*>;
	{t.deallocate(ptr,n)}->::std::same_as<void>;
	{t.reallocate(ptr,n)}->::std::same_as<typename T::value_type*>;
	{t.allocate_zero(n)}->::std::same_as<typename T::value_type*>;
};

template<typename T>
concept ncc_empty_allocator=::std::is_empty_v<T>&&ncc_allocator<T>;

}
