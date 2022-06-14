#include<fast_io_driver/timer.h>
#include<vector>

int main()
{
	std::vector<std::size_t> vec;
	{
		fast_io::timer tm(u8"std::vector::emplace_back");
		constexpr std::size_t n{100000000};
		vec.reserve(n);
		for(std::size_t i{};i!=n;++i)
		{
			vec.emplace_back(i);
		}
	}
	{
		fast_io::timer tm(u8"[]");
		std::size_t total_sum{};
		for(std::size_t i{};i!=vec.size();++i)
		{
			total_sum+=vec[i];
		}
	}
}