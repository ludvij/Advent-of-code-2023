#include <iostream>



int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const uint32_t res = 0;
	std::cout << "Total: " << res << '\n';
}