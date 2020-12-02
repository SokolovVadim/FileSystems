#include <vector>
#include <string>


class Node;
struct Pair
{
	Pair();
	int64_t key_;
	Node*   child_;
};

class Node
{
public:
	Node() = default;
	~Node() = default;
	Node(int degree);
	void insert(int64_t key);
private:
	uint32_t degree_;
	uint32_t keyNum_;
	bool isDeleted_;
	bool isLeaf_;
	std::vector<Pair> data_;
};
