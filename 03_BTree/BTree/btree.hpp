#pragma once
#include "node.hpp"

class BTree
{
public:
	BTree() = default;
	~BTree() = default;
	BTree(int degree);
	void insert(int64_t key);
	Node* search(int64_t key);
	void printTree();
	void printDot();
	void write_to_file(const std::string& msg);
private:
	int degree_;
	Node* root_;
};