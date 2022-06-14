#include<fast_io.h>
#include<fast_io_driver/timer.h>
#include<ncc_containers/vector.h>

int main()
{
	fast_io::timer tm(u8"ncc::vector::emplace_back_unchecked");
	ncc::vector<std::size_t> vec;
	constexpr std::size_t n{100000000};
	vec.reserve(n);
	for(std::size_t i{};i!=n;++i)
	{
		vec.emplace_back_unchecked(i);
	}
}