#include <iostream>
#include "node.hpp"
int main()
{
	std::cout << "Hello!\n";
	Node node();
	Node node0(10);
	for(int i(0); i < 10; ++i)
	{
		node0.insert(i);
	}
}