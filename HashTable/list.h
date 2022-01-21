#ifndef LIST_H
#define LIST_H

#include <iostream>

/********************************** DOUBLE SIDED LIST NODE IMPLEMENTATION **********************************/
/********************************** MODIFIED FOR USE IN HASH TABLE ****************************************/
template<class D>
class Node {
public:
	Node* prev;
	Node* next;
	D data;

	Node() = default;
	explicit Node(D _data) : prev(nullptr), next(nullptr), data(_data) {
		this->data = data;
	}

	~Node() {
		prev = nullptr;
		next = nullptr;
	}

	Node(const Node<D>& node) { //copy constructor
		prev = node.prev;
		next = node.next;
		data = node.data; //using constructor of class D
	}

	Node<D>& operator=(const Node<D>& node) {
		if (this == &node) {
			return *this;
		}
		prev = node.prev;
		next = node.next;
		data = data(node.data);
		return *this;
	}
};

/**********************************************************************************************/

template <class D>
class List {
	Node<D>* head; //dummy nodes
	Node<D>* tail;
	int size;

public:
	List();
	List(const List<D>& list); //copy constructor
	~List();
	void insertBeforeNode(D data, Node<D>* node);
	void insestAfterNode(D data, Node<D>* node);
	Node<D>* remove(Node<D>* node); //deletes node 
	Node<D>* pop_front(); //pops node without deleting
	void push_front(Node<D>* node); //pushes already existing node to the beginning of the list (doesnt create new node)
	Node<D>* find(D data);
	Node<D>* begin();
	Node<D>* end();
	Node<D>* getHead();
	Node<D>* getTail();
	int getSize();
	void destroyNode(Node<D>* to_delete);

};

/*LIST METHODS IMPLEMENTATIONS */
template<class D>
List<D>::List()
{
	
	Node<D>* tmp1 = new Node<D>( D() );
	Node<D>* tmp2 = new Node<D>( D() );
	head = tmp1;
	tail = tmp2;
	head->prev = nullptr;
	head->next = tail;
	tail->prev = head;
	tail->next = nullptr;
	size = 0;
}

template<class D>
List<D>::List(const List<D>& list)
{
	head = Node<D>(list.head);
	tail = Node<D>(list.tail);
	Node<D>* temp = (list.head)->next;
	Node<D>* current = head;
	while (temp != list.tail) {
		Node<D>* temp1 = Node<D>(temp);
		current->next = temp1;
		temp1->prev = current;
		temp = temp->next; //iteration
		current = current->next; //iteration
	}
	current->next = tail;
	tail->prev = current;
	size = list.size;
}

template <class D>
List<D>::~List() {
	Node<D>* temp = head->next;
	Node<D>* temp_next = temp->next;
	while (temp->next != nullptr) {
		delete temp;
		temp = temp_next; //iteration
		temp_next = temp->next; //iteration
	}
	delete tail;
	delete head;
	size = 0;
}

template<class D>
void List<D>::insertBeforeNode(D data, Node<D>* insert_before)
{
	Node<D>* new_node = new Node<D>(data);
	(insert_before->prev)->next = new_node;
	new_node->prev = insert_before->prev;
	new_node->next = insert_before;
	insert_before->prev = new_node;
	size++;
}

template<class D>
void List<D>::insestAfterNode(D data, Node<D>* insert_after)
{
	Node<D>* new_node = new Node<D>(data);
	(insert_after->next)->prev = new_node;
	new_node->next = (insert_after->next);
	insert_after->next = new_node;
	new_node->prev = insert_after;
	size++;
}

/*returns a pointer to node_to_delete->prev*/
template<class D>
Node<D>* List<D>::remove(Node<D>* node)
{
	Node<D>* tmp = node->prev;
	(node->prev)->next = node->next;
	(node->next)->prev = node->prev;
	size--;
	delete node;
	return tmp;
}

/*returns (head->next) and doesnt delete the node*/
template<class D>
Node<D>* List<D>::pop_front() {
	Node<D>* to_delete = head->next;
	head->next = to_delete->next;
	(to_delete->next)->prev = head;
	size--;
	return to_delete;
}

/*used to move nodes along lists
 pushes already existing node wo another list without having to copy beforehand*/
template<class D>
void List<D>::push_front(Node<D>* node) {
	node->next = head->next;
	node->prev = head;
	head->next = node;
	(node->next)->prev = node;
	size++;
}

/*returns pointer to node if found else returns nullptr*/
template<class D>
inline Node<D>* List<D>::find(D data)
{
	Node<D>* node = head;
	while (node->next != nullptr) {
		if (node->data == data) return node;
		node = node->next;
	}
	return nullptr;
}

template<class D>
Node<D>* List<D>::begin()
{
	return this->head->next;
}

template<class D>
Node<D>* List<D>::end()
{
	return this->tail->prev;
}

template<class D>
Node<D>* List<D>::getHead()
{
	return this->head;
}

template<class D>
Node<D>* List<D>::getTail()
{
	return this->tail;
}

template<class D>
inline int List<D>::getSize()
{
	return this->size;
}

template<class D>
void List<D>::destroyNode(Node<D>* to_delete)
{
	delete to_delete;
}

#endif // !LIST_H


