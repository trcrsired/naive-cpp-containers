#include<fast_io.h>
#include<fast_io_driver/timer.h>
#include<ncc_containers/vector.h>

int main()
{
	fast_io::timer t(u8"ncc::vector");
	ncc::vector<std::size_t> vec(100000000);
}