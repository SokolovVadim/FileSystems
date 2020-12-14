# FileSystems
File Systems course at MIPT by Department of Theoretical and Applied Informatics

## B-tree

##### Linux
```
cd 03_BTree
cmake -H. -Bbuild
cmake --build build
./build/BTree
```
##### Dot

```
dot -Tjpg out.dot -o out.jpg
display out.jpg
```

##### Example

```
	BTree tree(2);
	tree.insert(8);
	tree.insert(13);
	tree.insert(5);
	tree.insert(0);
	tree.insert(16);
	tree.insert(7);
	tree.insert(23);
	tree.insert(48);
	tree.insert(15);
	tree.insert(1);
	tree.insert(2);
	tree.insert(3);
	tree.insert(4);
	tree.printDot();
```
![dot](https://github.com/SokolovVadim/FileSystems/blob/main/03_BTree/resources/out.jpg)

