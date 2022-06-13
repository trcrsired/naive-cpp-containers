#pragma once

namespace ncc
{

template<typename T>
concept ncc_allocator=sizeof(T)<=(sizeof(void*)*2)&&::std::is_trivially_copyable_v<T>&&requires(T t,int* ptr,std::size_t n)
{
	{template t.allocate<int>(n)}->::std::same_as<int>;
	{template t.deallocate<int>(ptr,n)}->::std::same_as<void>;
	{template t.reallocate<int>(ptr,n)}->::std::same_as<int*>;
	{template t.allocate_zero<int>(n)}->::std::same_as<int*>;
};

template<typename T>
concept ncc_empty_allocator=::std::is_empty_v<T>&&ncc_allocator<T>;

}
