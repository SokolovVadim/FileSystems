#include <iostream>
#include <experimental/filesystem>
#include <vector>

// /proc/pid/stat

namespace fs = std::experimental::filesystem;

void IsDigit(const std::string & str, std::vector<fs::path>& dir_v)
{
	std::string digit = str.substr(6);
	// std::cout << digit << std::endl;
	std::string::const_iterator it = digit.begin();
    while (it != digit.end() && std::isdigit(*it)) ++it;
    if(!digit.empty() && it == digit.end())
    	dir_v.push_back(digit);
}

static int ps(char* argv[])
{
	const std::string path("/proc");

	std::vector<fs::path> dir_v;
	auto it = fs::directory_iterator(path);
	for(; it != fs::directory_iterator(); ++it)
    {
    	IsDigit(it->path(), dir_v);
        // std::cout << it1->path() << std::endl;
    }
    for(const fs::path& i:dir_v)
    {
        std::cout << i << "\n";
    }
	return 0;
}

int main(int argc, char* argv[])
{
	std::cout << "Args:\n";
	for(int i(0); i < argc; ++i)
		std::cout << argv[i] << std::endl;
	ps(argv);
	return 0;
}