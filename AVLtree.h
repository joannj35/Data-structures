#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>

enum class NodeType {
	LEAF,
	HAS_LEFT_CHILD_ONLY,
	HAS_RIGHT_CHILD_ONLY,
	HAS_TWO_CHILDREN
}; 

/********************************** TREE NODE IMPLEMENTATION **********************************/
template < class K, class V>
class Tnode {
public:
	Tnode* left;
	Tnode* right;
	Tnode* parent;
	V value;
	K key;  //same idea as STL MAP
	int height; //the length of the longest route from the current vertex to a leaf

	//default constructor
	Tnode() : left(nullptr), right(nullptr), parent(nullptr), value(NULL),
		key(NULL), height(0) {}

	//constructor
	Tnode(K key ,V value) : left(nullptr), right(nullptr), parent(nullptr), value(value),
		key(key), height(0) {}

	~Tnode() {
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}

	//copy
	Tnode(const Tnode<K, V>& node) {
		left = node.left;
		right = node.right;
		parent = node.parent;
		value = node.value;
		key = node.key;
		height = node.height;
	}

	//operators
	Tnode<K, V>& operator= (const Tnode<K, V>& node) {
		if (this == &node) {
			return *this;
		}
		left = node.left;
		right = node.right;
		parent = node.parent;
		value = node.value;
		key = node.key;
		height = node.height;
		return *this;
	}

	bool operator==(const Tnode<K,V>& node) const {
		return key == node->key;
	}

	//Balance Factor
	int getBF() const { //BF in range [-2,2]
		if (left != nullptr && right != nullptr) {
			return (left->height - right->height);
		}
		else if (left != nullptr && right == nullptr) {
			return (left->height + 1);
		}
		else if (left == nullptr && right != nullptr) {
			return (-1) * (right->height + 1);
		}
		else
			return 0;
	}
};

/**********************************AVL TREE IMPLEMENTATION **********************************/
template <class K, class V>
class AVLtree {
	Tnode<K, V>* root;
	int size;

public:
	AVLtree() : root(nullptr), size(0) {};
	~AVLtree();
	AVLtree(const AVLtree<K, V>& tree);
	AVLtree<K, V>& operator=(const AVLtree<K, V>& tree);
	int getSize();
	Tnode<K, V>* getRoot();
	void setRoot(Tnode<K, V>* new_root);
	Tnode<K, V>* find(K key,Tnode<K,V>* current);
	Tnode<K, V>* insert(K key, V value); //inserts node while maintainig AVL rules 
	bool remove(K key,Tnode<K,V>* node); //MUST MAINTAIN AVL RULES AFTER REMOVING (ROTATE)
	void rotateLR(Tnode<K, V>* node);
	void rotateLL(Tnode<K, V>* node);
	void rotateRL(Tnode<K, V>* node);
	void rotateRR(Tnode<K, V>* node);
	void printInOrder(Tnode<K, V>* current);
	Tnode<K, V>* swapTwoNodes(Tnode<K, V>* node);

};

/*AVL TREE FUNCTION IMPLEMENTATIONS*/

//destructor
template<class K, class V>
static void treeDestructorAUX(Tnode<K, V>* node) {
	if (node == nullptr) {
		return;
	}
	treeDestructorAUX(node->left);
	treeDestructorAUX(node->right);
	delete node;
}

template<class K, class V>
inline AVLtree<K, V>::~AVLtree()
{
	treeDestructorAUX(this->root);
}

//copy connstrucor
template<class K, class V>
static Tnode<K,V>* treeCopyAUX(Tnode<K, V>* src_node) { 
	if (src_node == nullptr) { //reached a leaf
		return nullptr;
	}
	Tnode<K, V>* dest_node = new Tnode<K, V>(*src_node);
	dest_node->left = treeCopyAUX(src_node->left);
	dest_node->right = treeCopyAUX(src_node->right);
	if (src_node->left != nullptr) { //if left son exists, set parent
		(dest_node->left)->parent = dest_node;
	}
	if (src_node->right != nullptr) {
		(dest_node->right)->parent = dest_node;
	}
	return dest_node;
}

template<class K, class V>
AVLtree<K, V>::AVLtree(const AVLtree<K, V>& tree)
{
	root = treeCopyAUX(tree.root);
	if (root != nullptr) {
		root->parent = nullptr;
		size = tree.size;
	}	
	else
		size = 0;
}

template<class K, class V>
AVLtree<K, V>& AVLtree<K, V>::operator=(const AVLtree<K, V>& tree)
{
	if (this == &tree) {
		return *this;
	}
	if (tree.root != nullptr) {
		root = treeCopyAUX(tree.root);
		root->parent = nullptr;
		size = tree.size;
	}
	else 
		size = 0;
	return *this;
}

template<class K, class V>
int AVLtree<K, V>::getSize()
{
	return size;
}

template<class K, class V>
Tnode<K, V>* AVLtree<K, V>::getRoot()
{
	return root;
}

template<class K, class V>
void AVLtree<K, V>::setRoot(Tnode<K, V>* new_root)
{
	root = new_root;
}

template<class K, class V>
Tnode<K, V>* AVLtree<K, V>::find(K key, Tnode<K,V>* current)
{
	if(current == nullptr) return nullptr;
	if (key == current->key) return current;
	if (key > current->key) {
		return find(key, current->right);
	}
	return find(key, current->left);
}

/*this function updates the node's height field*/
template<class K, class V>
static void updateNodeHeight(Tnode<K, V>* node) {
	int hL = -1, hR = -1;
	if (node == nullptr) {
		//throw std::exception("Err: Node is a nullptr");
	}
	if (node->left != nullptr) {
		hL = (node->left)->height;
	}
	if (node->right != nullptr) {
		hR = (node->right)->height;
	}
	if (hR > hL) {
		node->height = hR + 1;
	}
	else
		node->height = hL + 1;
}

/* this function updates the heights of all nodes in path from current node up to the root and rotates*/
template<class K, class V>
static void updatePathHeight(Tnode<K, V>* node, AVLtree<K, V>* tree) {
	while (node != nullptr) { 
		updateNodeHeight(node);
		if (node->getBF() == 2 || node->getBF() == -2) {
			rotate(tree, node);
			if (tree->getRoot()->parent != nullptr) {
				 tree->setRoot(tree->getRoot()->parent);
			}
		}
		node = node->parent; //working our way up to the root
	}
}

template<class K, class V>
static NodeType getNodeType(Tnode<K, V>* node) {
	if (node->left == nullptr && node->right == nullptr) {
		return NodeType::LEAF;
	}

	if (node->left != nullptr && node->right == nullptr) {
		return NodeType::HAS_LEFT_CHILD_ONLY;
	}

	if (node->left == nullptr && node->right != nullptr) {
		return NodeType::HAS_RIGHT_CHILD_ONLY;
	}

	return NodeType::HAS_TWO_CHILDREN;
}

template<class K, class V>
Tnode<K,V>* AVLtree<K, V>::insert(K key, V value)
{
	Tnode<K, V>* new_node = new Tnode<K, V>(key, value);
	if (new_node == nullptr) {
		//throw std::exception("Memory Error!");
	}
	if (root == nullptr) { //tree is empty add root
		root = new_node;
		size++;
		return root;
	}
	Tnode<K, V>* current =  root;
	while (current != nullptr) { //while current != leaf
		if (key == current->key) { //key already exists
			return current;
		}
		if (key < current->key && current->left != nullptr) {
			current = current->left;
		}
		if (key < current->key && current->left == nullptr) {
			current->left = new_node; //left insertion 
			new_node->parent = current;
			size++;
			break;
		}
		if (key > current->key && current->right != nullptr) {
			current = current->right;
		}
		if(key > current->key && current->right == nullptr) {
			current->right = new_node; //right insertion
			new_node->parent = current;
			size++;
			break;
		}
	}
	updatePathHeight(current, this); //update node heights up to the root
	return new_node;
}

/*parameter node is the root of the tree/subtree the binary search should start from*/
template<class K, class V>
bool AVLtree<K, V>::remove(K key, Tnode<K,V>* node)
{
	Tnode<K, V>* node_remove = find(key, node);
	Tnode<K, V>* swapWith;
	if (node_remove == nullptr ) return false; //didnt find node in tree
	
	Tnode<K, V>* node_parent = node_remove->parent;
	switch (getNodeType(node_remove)) {
	case NodeType::LEAF:
		if (node_parent == nullptr) { //node is the root
			this->root = nullptr;
			delete node_remove;
			size--;
			return true;
		}
		if (node_parent->left == node_remove) { //node is left leaf
			node_parent->left = nullptr;
		}
		else { //node is right leaf
			node_parent->right = nullptr; 
		}
		delete node_remove; //delete node
		updatePathHeight(node_parent, this);
		size--;
		return true;
	case NodeType::HAS_LEFT_CHILD_ONLY:
		if (node_remove == this->root) { //the node to be deleted is the root
			(node_remove->left)->parent = nullptr; 
			this->root = node_remove->left;
			delete node_remove;
		}
		else { 
			if (node_parent->left == node_remove) { //LL
				node_parent->left = node_remove->left; 
				(node_remove->left)->parent = node_parent;
				updateNodeHeight(node_parent);
				delete node_remove;
			}
			else { 
				node_parent->right = node_remove->left;//RL
				(node_remove->left)->parent = node_parent;
				updateNodeHeight(node_parent);
				delete node_remove;
			}
		}
		size--;
		return true;
	case NodeType::HAS_RIGHT_CHILD_ONLY:
		if (node_remove == this->root) { //the node to be deleted is the root
			(node_remove->right)->parent = nullptr;
			this->root = node_remove->right;
			delete node_remove;
		}
		else {
			if (node_parent->right == node_remove) { //RR
				node_parent->right = node_remove->right;
				(node_remove->right)->parent = node_parent;
				updateNodeHeight(node_parent);
				delete node_remove;
			}
			else {
				node_parent->left = node_remove->right; //LR
				(node_remove->right)->parent = node_parent;
				updateNodeHeight(node_parent);
				delete node_remove;
			}
		}
		size--;
		return true;
	case NodeType::HAS_TWO_CHILDREN:
		swapWith = swapTwoNodes(node_remove);
		if (remove(key,swapWith->right)) {
			return true;
		}
	}
	return false;
}

/*parameters: the node to be swapped before deletion in func remove*/
/*find the node which should be deleted, mark its right son, find the lowest key of the son's LEFT sided sons
   then swap between the node and the farthest left node found, update left+right sons and parents accordingly
   function returns pointer to the node we want to delete*/
template<class K, class V>
Tnode<K,V>* AVLtree<K, V>::swapTwoNodes(Tnode<K,V>* node)
{
	Tnode<K, V>* swapWith = node->right;
	//Tnode<K, V> temp = *node;
	if (node == nullptr) {
		//throw std::exception("Node is nullptr");
	}

	//find farthest left node in right sided son's subtree
	while (swapWith->left != nullptr) 
	{
		swapWith = swapWith->left;
	}
	if (node->right->key != swapWith->key) {
		Tnode<K, V>* temp = node->parent;
		node->parent = swapWith->parent;
		swapWith->parent = temp;
		if (swapWith->parent == nullptr) {
			setRoot(swapWith);
		}
		temp = node->left;
		node->left = swapWith->left;
		swapWith->left = temp;
		if (node->left != nullptr) {
			node->left->parent = node;
		}
		swapWith->left->parent = swapWith;
		temp = node->right;
		node->right = swapWith->right;
		swapWith->right = temp;
		if (node->right != nullptr) {
			node->right->parent = node;
		}
		swapWith->right->parent = swapWith;
		if (swapWith->parent != nullptr) {
			if (swapWith->parent->key > node->key) {
				swapWith->parent->left = swapWith;
			}
			else {
				swapWith->parent->right = swapWith;
			}
		}
		if (node->parent->key > swapWith->key) {
			node->parent->left = node;
		}
		else {
			node->parent->right = node;
		}
	}
	else {
		swapWith->parent = node->parent;
		node->parent = swapWith;
		if (swapWith->parent == nullptr) {
			setRoot(swapWith);
		}
		Tnode<K, V>* temp = node->left;
		node->left = swapWith->left;
		swapWith->left = temp;
		if (node->left != nullptr) {
			node->left->parent = node;
		}
		swapWith->left->parent = swapWith;
		node->right = swapWith->right;
		swapWith->right = node;
		if (node->right != nullptr) {
			node->right->parent = node;
		}
		swapWith->right->parent = swapWith;
		if (swapWith->parent != nullptr) {
			if (swapWith->parent->key > node->key) {
				swapWith->parent->left = swapWith;
			}
			else {
				swapWith->parent->right = swapWith;
			}
		}
		if (node->parent->key > swapWith->key) {
			node->parent->left = node;
		}
		else {
			node->parent->right = node;
		}
	}
	int temp_height = node->height;
	node->height = swapWith->height;
	swapWith->height = temp_height;
	//swap nodes
	/*
	node->key = swapWith->key;
	node->value = swapWith->value;
	swapWith->key = temp.key;
	swapWith->value = temp.value;*/
	return swapWith;
	
}


template<class K, class V>
void AVLtree<K, V>::rotateLR(Tnode<K, V>* current)
{
	if (current == nullptr) {
		//throw std::exception("Err: node is a nullptr");
	}
	rotateRR(current->left);
	rotateLL(current);
}

template<class K, class V>
void AVLtree<K, V>::rotateRL(Tnode<K, V>* current)
{
	if (current == nullptr) {
		//throw std::exception("Err: node is a nullptr");
	}
	rotateLL(current->right);
	rotateRR(current);
}


template<class K, class V>
 static void rotate(AVLtree<K, V>* tree, Tnode<K, V>* current) {
 	 if (current->getBF() == 2 && (current->left) != nullptr && (current->left)->getBF() >= 0) {
		tree->rotateLL(current);
	}
	else if (current->getBF() == 2 && (current->left) != nullptr && (current->left)->getBF() == -1) {
		 tree->rotateLR(current);
	}
	else if (current->getBF() == -2 && (current->right) != nullptr && (current->right)->getBF() == 1) {
		 tree->rotateRL(current);
	}
	else if (current->getBF() == -2 && (current->right) != nullptr && (current->right)->getBF() <= 0) {
		 tree->rotateRR(current);
	}
}
template<class K, class V>
void AVLtree<K, V>::rotateLL(Tnode<K, V>* current)
{
	if (current == nullptr) {
		//throw std::exception("Exeption: node is a nullptr");
	}
	Tnode<K, V>* temp = current->left;//A
	if (temp->right != nullptr) {
		current->left = temp->right; //AR
		(temp->right)->parent = current;
	}
	else{
		current->left = nullptr;
	}
	temp->right = current;//AR=B
	temp->parent = current->parent;

	if (current->parent != nullptr) {// updating current nodes parent
		Tnode<K, V>* parent = current->parent;
		if (parent->left == current) {
			parent->left = temp;
		}
		else {
			parent->right = temp;
		}
	}
	current->parent = temp;
	//updating heights
	if (temp->right != nullptr) {
		updateNodeHeight(temp->right);
	}
	updateNodeHeight(temp);
	updateNodeHeight(current);
	if (current->parent != nullptr) {
		updateNodeHeight(current->parent);
	}
}

template<class K, class V>
void AVLtree<K, V>::rotateRR(Tnode<K, V>* current)
{
	if (current == nullptr) {
		//throw std::exception("Err: node is a nullptr");
	}
	Tnode<K, V>* temp = current->right;
	if (temp->left != nullptr) {
		current->right = temp->left;
		(temp->left)->parent = current;
	}
	else {
		current->right = nullptr;
	}
	temp->left = current;
	temp->parent = current->parent;
	
	if (current->parent != nullptr) {// updating current nodes parent
		Tnode<K, V>* parent = current->parent;
		if (parent->left == current) {
			parent->left = temp;
		}
		else {
			parent->right = temp;
		}
	}
	current->parent = temp;
	//updating heights
	if (temp->left != nullptr) {
		updateNodeHeight(temp->left);
	}
	updateNodeHeight(temp);
	updateNodeHeight(current);
	if (current->parent != nullptr) {
		updateNodeHeight(current->parent);
	}
}

template<class K, class V>
void AVLtree<K, V>::printInOrder(Tnode<K, V>* current)
{
	if (current == nullptr) return;
	printInOrder(current->left);
	std::cout << current->key << " ";
	printInOrder(current->right);
	
}

#endif //AVLTREE_H

