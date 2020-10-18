#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

// /proc/pid/stat

struct Stat
{
	pid_t pid;
	std::string comm;
	char state;
	std::string utime;
};

namespace fs = std::experimental::filesystem;

bool IsDigit(const std::string & str, std::vector<fs::path>& dir_v)
{
	bool isdigit(false);
	std::string digit = str.substr(6);
	// std::cout << digit << std::endl;
	std::string::const_iterator it = digit.begin();
    while (it != digit.end() && std::isdigit(*it)) ++it;
    if(!digit.empty() && it == digit.end())
    {
    	isdigit = true;
    	dir_v.push_back(digit);
    }
    return isdigit;
}

std::string GatherInfo(const fs::path& path)
{
	std::string f_path(std::string(path) + "/stat");
	// std::cout << "f_path: " << f_path << std::endl;
	std::ifstream stat_file{};
	stat_file.open(f_path.c_str());

	std::stringstream strStream;
    strStream << stat_file.rdbuf(); //read the file
    std::string str = strStream.str();

    // std::cout << "str: " << str << std::endl; 
	stat_file.close();
	return str;
}

Stat ParseStat(const std::string& stat_info)
{
	std::vector<std::string> v;

	std::string delimiter = " ";
	size_t last = 0;
	size_t next = 0;
	while ((next = stat_info.find(delimiter, last)) != std::string::npos) {
	   std::string token = stat_info.substr(last, next-last);
	   v.push_back(token);
	   last = next + 1;
	} 
	v.push_back(stat_info.substr(last));

	Stat stat_struct = {
		.pid = std::stoi(v[0]),
		.comm = v[1],
		.state = v[2][0],
		.utime = v[13]
	};
	return stat_struct;
}

void PrintStat(const std::vector<Stat>& stat_v)
{
	std::cout << "Pid\tcomm\t\tstate\tstarttime\n";
	for(auto i: stat_v)
	{
		std::cout << i.pid << "\t" << i.comm << "\t" << i.state << "\t" << i.utime << std::endl;
	}
}

static int ps(char* argv[])
{
	const std::string path("/proc");

	std::vector<fs::path> dir_v;
	std::vector<Stat> stat_v;
	auto it = fs::directory_iterator(path);
	for(; it != fs::directory_iterator(); ++it)
    {
    	if(IsDigit(it->path(), dir_v))
    	{
    		std::string stat_info = GatherInfo(it->path());
    		stat_v.push_back(ParseStat(stat_info));
    	}
    }
    PrintStat(stat_v);
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