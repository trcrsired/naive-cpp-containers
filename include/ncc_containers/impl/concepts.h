#pragma once

namespace ncc
{

template<typename T>
concept ncc_allocator=sizeof(T)<=(sizeof(void*)*2)
	&&::std::is_aggregate_v<T>&&::std::is_trivially_copyable_v<T>&&requires(T t,int* ptr,std::size_t n)
{
	{t.template allocate<int>(n)}->::std::same_as<int>;
	{t.template deallocate<int>(ptr,n)}->::std::same_as<void>;
	{t.template reallocate<int>(ptr,n)}->::std::same_as<int*>;
	{t.template allocate_zero<int>(n)}->::std::same_as<int*>;
};

template<typename T>
concept ncc_empty_allocator=::std::is_empty_v<T>&&ncc_allocator<T>;

}
