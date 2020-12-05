#ifndef NODE_HPP
#define NODE_HPP
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
	void split(Node* child, int idx, int64_t key);
	Node* search(int64_t key);
	void shift(int64_t key, int64_t idx);
private:
	uint32_t degree_;
	uint32_t keyNum_;
	bool isDeleted_;
	bool isLeaf_;
	std::vector<Pair> data_;
};

#endif // NODE_HPP