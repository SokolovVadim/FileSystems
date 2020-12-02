#include "node.hpp"

class BTree
{
public:
	BTree() = default;
	~BTree() = default;
	BTree(int degree);
	void insert(int key);
private:
	int degree_;
	Node* root_;
};