#include<ncc_containers/vector.h>
#include<cstddef>

int main()
{
	ncc::vector<std::size_t> vec;
	vec.emplace_back(20);
}
