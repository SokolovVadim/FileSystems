#ifndef NODE_HPP
#define NODE_HPP
#include <vector>
#include <string>


class Node;
struct Pair
{
	Pair();
	int64_t key_;
	Node* child_;
};

struct Triple
{
	Triple(int64_t key, Node* left, Node* right):
		key_(key),
		leftChild_(left),
		rightChild_(right)
	{}
	int64_t key_;
	Node* leftChild_;
	Node* rightChild_;
};

class Node
{
public:
	Node() = default;
	~Node() = default;
	Node(int degree, bool isLeaf);
	void insert(int64_t key);
	void splitChild(int idx, Node* child);
	Node* search(int64_t key);
	void shift_right(int64_t key, int64_t idx);
	void shift_left(int64_t key);
	const int64_t getKeyNum() const;
	bool isNodeFull();
	const bool isLeaf() const;
	Node* splitRoot(int64_t key);
	void printNode();
	void printAll();
	void printDot(std::string& msg);
private:
	uint64_t degree_;
	uint64_t keyNum_;
	bool isDeleted_;
	bool isLeaf_;
	std::vector<Pair> data_;
};

#endif // NODE_HPP