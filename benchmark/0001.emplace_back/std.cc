#include<fast_io.h>
#include<fast_io_driver/timer.h>
#include<vector>

int main()
{
	fast_io::timer tm(u8"std::vector::emplace_back");
	std::vector<std::size_t> vec;
	constexpr std::size_t n{100000000zu};
	vec.reserve(n);
	for(std::size_t i{};i!=n;++i)
	{
		vec.emplace_back(i);
	}
}